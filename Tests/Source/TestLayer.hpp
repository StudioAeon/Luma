#pragma once

#include "Luma.hpp"

namespace Luma {

	class Texture2D;
	class Shader;
	class VertexArray;
	class Framebuffer;

	struct TestResult
	{
		std::string Name;
		bool Passed = true;
		std::string Message;
		float ExecutionTime = 0.0f;
	};

	class Test
	{
	public:
		virtual ~Test() = default;
		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual TestResult Run() = 0;
		virtual const char* GetName() const = 0;
		virtual const char* GetCategory() const = 0;
	};

	class TestLayer : public Layer
	{
	public:
		TestLayer();
		virtual ~TestLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		void RenderMainMenuBar();
		void RenderTestListPanel();
		void RenderTestDetailsPanel();
		void RenderPerformancePanel();
		void RenderEventLogPanel();
		void RenderRendererStatsPanel();

		void RunTest(size_t index);
		void RunAllTests();
		void ClearResults();

		template<typename T>
		void RegisterTest()
		{
			static_assert(std::is_base_of_v<Test, T>, "T must derive from Test");
			m_Tests.push_back(std::make_unique<T>());
		}

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void LogEvent(const std::string& message);
		void UpdatePerformanceMetrics(Timestep ts);

	private:
		std::vector<std::unique_ptr<Test>> m_Tests;
		std::vector<TestResult> m_Results;
		int m_SelectedTestIndex = -1;
		bool m_AutoRunTests = false;
		bool m_ShowPassedTests = true;
		bool m_ShowFailedTests = true;

		bool m_ShowTestList = true;
		bool m_ShowTestDetails = true;
		bool m_ShowPerformance = true;
		bool m_ShowEventLog = true;
		bool m_ShowRendererStats = true;

		std::vector<std::string> m_EventLog;
		static constexpr size_t MaxEventLogSize = 100;

		struct PerformanceMetrics
		{
			float FrameTime = 0.0f;
			float UpdateTime = 0.0f;
			float RenderTime = 0.0f;
			uint32_t FrameCount = 0;
			std::vector<float> FrameTimeHistory;
			static constexpr size_t HistorySize = 100;
		} m_Metrics;
	};

}