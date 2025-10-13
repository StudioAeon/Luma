#include "TestLayer.hpp"

#include "Tests/EventSystemTests.hpp"
#include "Tests/InputTests.hpp"
#include "Tests/MemoryTests.hpp"
#include "Tests/SerializationTests.hpp"
#include "Tests/RendererTests.hpp"

#include <imgui.h>

#include <chrono>
#include <format>

namespace Luma {

	TestLayer::TestLayer() : Layer("TestLayer") {}
	TestLayer::~TestLayer() {}

	void TestLayer::OnAttach()
	{
		LM_INFO("TestLayer attached");
		LogEvent("TestLayer initialized");

		RegisterTest<EventDispatchTest>();
		RegisterTest<EventQueueTest>();

		RegisterTest<InputStateTest>();
		RegisterTest<InputPollingTest>();

		RegisterTest<RefCountTest>();
		RegisterTest<BufferTest>();

		RegisterTest<YAMLSerializationTest>();
		RegisterTest<BinarySerializationTest>();

		RegisterTest<RendererInitTest>();
		RegisterTest<TextureLoadTest>();
		RegisterTest<ShaderCompileTest>();
		RegisterTest<FramebufferTest>();
		RegisterTest<VertexBufferTest>();
		RegisterTest<RenderCommandTest>();

		for (auto& test : m_Tests)
			test->OnInit();

		m_Metrics.FrameTimeHistory.reserve(PerformanceMetrics::HistorySize);
	}

	void TestLayer::OnDetach()
	{
		for (auto& test : m_Tests)
			test->OnShutdown();

		LM_INFO("TestLayer detached");
	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		auto start = std::chrono::high_resolution_clock::now();

		UpdatePerformanceMetrics(ts);

		if (m_SelectedTestIndex >= 0 && m_SelectedTestIndex < m_Tests.size())
			m_Tests[m_SelectedTestIndex]->OnUpdate(ts);

		Renderer::Clear();

		auto end = std::chrono::high_resolution_clock::now();
		m_Metrics.UpdateTime = std::chrono::duration<float, std::milli>(end - start).count();
	}

	void TestLayer::OnImGuiRender()
	{
		auto start = std::chrono::high_resolution_clock::now();

		RenderMainMenuBar();

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", nullptr, flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("TestDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		ImGui::End();

		if (m_ShowTestList) RenderTestListPanel();
		if (m_ShowTestDetails) RenderTestDetailsPanel();
		if (m_ShowPerformance) RenderPerformancePanel();
		if (m_ShowEventLog) RenderEventLogPanel();
		if (m_ShowRendererStats) RenderRendererStatsPanel();

		if (m_SelectedTestIndex >= 0 && m_SelectedTestIndex < m_Tests.size())
		{
			ImGui::Begin("Test Visualization");
			m_Tests[m_SelectedTestIndex]->OnImGuiRender();
			ImGui::End();
		}

		auto end = std::chrono::high_resolution_clock::now();
		m_Metrics.RenderTime = std::chrono::duration<float, std::milli>(end - start).count();
	}

	void TestLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(LM_BIND_EVENT_FN(TestLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(LM_BIND_EVENT_FN(TestLayer::OnMouseButtonPressed));
	}

	void TestLayer::RenderMainMenuBar()
	{
		if (!ImGui::BeginMainMenuBar())
			return;

		if (ImGui::BeginMenu("Tests"))
		{
			if (ImGui::MenuItem("Run All Tests", "Ctrl+R"))
				RunAllTests();
			if (ImGui::MenuItem("Clear Results"))
				ClearResults();
			ImGui::Separator();
			ImGui::MenuItem("Auto-run on Startup", nullptr, &m_AutoRunTests);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Test List", nullptr, &m_ShowTestList);
			ImGui::MenuItem("Test Details", nullptr, &m_ShowTestDetails);
			ImGui::MenuItem("Performance", nullptr, &m_ShowPerformance);
			ImGui::MenuItem("Event Log", nullptr, &m_ShowEventLog);
			ImGui::MenuItem("Renderer Stats", nullptr, &m_ShowRendererStats);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Filter"))
		{
			ImGui::MenuItem("Show Passed", nullptr, &m_ShowPassedTests);
			ImGui::MenuItem("Show Failed", nullptr, &m_ShowFailedTests);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	void TestLayer::RenderTestListPanel()
	{
		ImGui::Begin("Test List", &m_ShowTestList);

		ImGui::Text("Total Tests: %zu", m_Tests.size());
		ImGui::SameLine();
		if (ImGui::Button("Run All"))
			RunAllTests();

		ImGui::Separator();

		std::unordered_map<std::string, std::vector<size_t>> categories;
		for (size_t i = 0; i < m_Tests.size(); ++i)
			categories[m_Tests[i]->GetCategory()].push_back(i);

		for (const auto& [category, indices] : categories)
		{
			if (!ImGui::TreeNodeEx(category.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				continue;

			for (size_t idx : indices)
			{
				const auto& test = m_Tests[idx];
				bool hasResult = idx < m_Results.size();

				if (hasResult)
				{
					ImGui::PushStyleColor(ImGuiCol_Text,
						m_Results[idx].Passed ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				}

				bool isSelected = (m_SelectedTestIndex == static_cast<int>(idx));
				if (ImGui::Selectable(test->GetName(), isSelected))
					m_SelectedTestIndex = static_cast<int>(idx);

				if (hasResult)
					ImGui::PopStyleColor();

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Run Test"))
						RunTest(idx);
					ImGui::EndPopup();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}

	void TestLayer::RenderTestDetailsPanel()
	{
		ImGui::Begin("Test Details", &m_ShowTestDetails);

		if (m_SelectedTestIndex < 0 || m_SelectedTestIndex >= m_Tests.size())
		{
			ImGui::TextDisabled("No test selected");
			ImGui::End();
			return;
		}

		const auto& test = m_Tests[m_SelectedTestIndex];

		ImGui::Text("Name: %s", test->GetName());
		ImGui::Text("Category: %s", test->GetCategory());
		ImGui::Separator();

		if (ImGui::Button("Run This Test"))
			RunTest(m_SelectedTestIndex);

		if (m_SelectedTestIndex < m_Results.size())
		{
			ImGui::Separator();
			const auto& result = m_Results[m_SelectedTestIndex];

			ImGui::Text("Status: ");
			ImGui::SameLine();
			ImGui::TextColored(
				result.Passed ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
				result.Passed ? "PASSED" : "FAILED");

			ImGui::Text("Execution Time: %.3f ms", result.ExecutionTime);

			if (!result.Message.empty())
			{
				ImGui::Separator();
				ImGui::TextWrapped("Message: %s", result.Message.c_str());
			}
		}

		ImGui::End();
	}

	void TestLayer::RenderPerformancePanel()
	{
		ImGui::Begin("Performance", &m_ShowPerformance);

		ImGui::Text("Frame: %u", m_Metrics.FrameCount);
		ImGui::Text("Frame Time: %.2f ms (%.1f FPS)", m_Metrics.FrameTime, 1000.0f / m_Metrics.FrameTime);
		ImGui::Text("Update Time: %.2f ms", m_Metrics.UpdateTime);
		ImGui::Text("Render Time: %.2f ms", m_Metrics.RenderTime);
		ImGui::Separator();

		if (!m_Metrics.FrameTimeHistory.empty())
		{
			ImGui::PlotLines("Frame Time (ms)",
				m_Metrics.FrameTimeHistory.data(),
				static_cast<int>(m_Metrics.FrameTimeHistory.size()),
				0, nullptr, 0.0f, 50.0f, ImVec2(0, 80));
		}

		ImGui::End();
	}

	void TestLayer::RenderEventLogPanel()
	{
		ImGui::Begin("Event Log", &m_ShowEventLog);

		if (ImGui::Button("Clear Log"))
			m_EventLog.clear();

		ImGui::Separator();
		ImGui::BeginChild("EventLogScroll");

		for (const auto& event : m_EventLog)
			ImGui::TextUnformatted(event.c_str());

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

	void TestLayer::RenderRendererStatsPanel()
	{
		ImGui::Begin("Renderer Stats", &m_ShowRendererStats);

		auto& caps = RendererAPI::GetCapabilities();
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Renderer.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::Separator();

		ImGui::Text("Draw Calls: %d", 0);
		ImGui::Text("Vertices: %d", 0);
		ImGui::Text("Triangles: %d", 0);

		ImGui::End();
	}

	void TestLayer::RunTest(size_t index)
	{
		if (index >= m_Tests.size())
			return;

		auto start = std::chrono::high_resolution_clock::now();
		TestResult result = m_Tests[index]->Run();
		auto end = std::chrono::high_resolution_clock::now();

		result.ExecutionTime = std::chrono::duration<float, std::milli>(end - start).count();

		if (index >= m_Results.size())
			m_Results.resize(m_Tests.size());

		m_Results[index] = result;

		LogEvent(std::format("Test '{}' {}: {}",
			result.Name,
			result.Passed ? "PASSED" : "FAILED",
			result.Message));
	}

	void TestLayer::RunAllTests()
	{
		LogEvent("Running all tests...");
		m_Results.clear();
		m_Results.resize(m_Tests.size());

		for (size_t i = 0; i < m_Tests.size(); ++i)
			RunTest(i);

		size_t passed = 0, failed = 0;
		for (const auto& result : m_Results)
		{
			if (result.Passed) passed++;
			else failed++;
		}

		LogEvent(std::format("All tests complete: {} passed, {} failed", passed, failed));
	}

	void TestLayer::ClearResults()
	{
		m_Results.clear();
		LogEvent("Test results cleared");
	}

	bool TestLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == KeyCode::R && Input::IsKeyPressed(KeyCode::LeftControl))
		{
			RunAllTests();
			return true;
		}

		return false;
	}

	bool TestLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void TestLayer::LogEvent(const std::string& message)
	{
		m_EventLog.push_back(message);

		if (m_EventLog.size() > MaxEventLogSize)
			m_EventLog.erase(m_EventLog.begin());
	}

	void TestLayer::UpdatePerformanceMetrics(Timestep ts)
	{
		m_Metrics.FrameTime = ts.GetMilliseconds();
		m_Metrics.FrameCount++;

		m_Metrics.FrameTimeHistory.push_back(m_Metrics.FrameTime);
		if (m_Metrics.FrameTimeHistory.size() > PerformanceMetrics::HistorySize)
			m_Metrics.FrameTimeHistory.erase(m_Metrics.FrameTimeHistory.begin());
	}

}