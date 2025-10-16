#include "lmpch.hpp"
#include "Application.hpp"

#include "Luma/Renderer/Renderer.hpp"
#include "Luma/Renderer/Framebuffer.hpp"

#include "Input.hpp"
#include "FatalSignal.hpp"

#include "Luma/Asset/AssetManager.hpp" //Temp
#include "Luma/Renderer/Backend/Vulkan/VulkanAllocator.hpp"
#include "Luma/Renderer/RendererAPI.hpp"

#include "Luma/Utilities/StringUtils.hpp"
#include "Luma/Debug/Profiler.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <glad/glad.h>

#include <nfd.hpp>

#include <filesystem>

bool g_ApplicationRunning = true;
namespace Luma {

	Application* Application::s_Instance = nullptr;

	static std::thread::id s_MainThreadID;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		FatalSignal::Install();

		s_Instance = this;
		s_MainThreadID = std::this_thread::get_id();

		if (!specification.WorkingDirectory.empty())
			std::filesystem::current_path(specification.WorkingDirectory);

		m_Profiler = lnew PerformanceProfiler();

		WindowSpecification windowSpec;
		windowSpec.Title = specification.Name;
		windowSpec.Width = specification.WindowWidth;
		windowSpec.Height = specification.WindowHeight;
		windowSpec.Mode = specification.Mode;
		windowSpec.VSync = specification.VSync;
		m_Window = Window::Create(windowSpec);
		m_Window->Init();
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

		LM_CORE_VERIFY(NFD::Init() == NFD_OKAY);

		// Init renderer and execute command queue to compile all shaders
		Renderer::Init();
		Renderer::WaitAndRender();

		m_ImGuiLayer = ImGuiLayer::Create();
		PushOverlay(m_ImGuiLayer);

		m_Window->SetResizable(specification.Resizable);
		if (windowSpec.Mode == WindowMode::Windowed)
			m_Window->CenterWindow();

		AssetManager::Init();
	}

	Application::~Application()
	{
		NFD::Quit();

		m_Window->SetEventCallback([](Event& e) {});

		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		FramebufferPool::GetGlobal()->GetAll().clear();

		AssetManager::Shutdown();

		Renderer::WaitAndRender();
		Renderer::Shutdown();

		m_Window->Shutdown();

		delete m_Profiler;
		m_Profiler = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::RenderImGui()
	{
		LM_PROFILE_FUNC();
		LM_SCOPE_PERF("Application::RenderImGui");

		m_ImGuiLayer->Begin();

		ImGui::Begin("Renderer");
		auto& caps = Renderer::GetCapabilities();

		if (RendererAPI::Current() == RendererAPIType::Vulkan)
		{
			GPUMemoryStats memoryStats = VulkanAllocator::GetStats();
			std::string used = Utils::BytesToString(memoryStats.Used);
			std::string free = Utils::BytesToString(memoryStats.Free);
			ImGui::Text("Used VRAM: %s", used.c_str());
			ImGui::Text("Free VRAM: %s", free.c_str());
		}

		ImGui::End();

		for (int i = 0; i < m_LayerStack.Size(); i++)
			m_LayerStack[i]->OnImGuiRender();
	}

	void Application::SyncEvents()
	{
		std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
		for (auto& [synced, _] : m_EventQueue)
		{
			synced = true;
		}
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			static uint64_t frameCounter = 0;

			ProcessEvents();

			if (!m_Minimized)
			{
				Renderer::BeginFrame();
				{
					LM_SCOPE_PERF("Application Layer::OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(m_TimeStep);
				}

				// Render ImGui on render thread
				Application* app = this;
				Renderer::Submit([app]() { app->RenderImGui(); });
				Renderer::Submit([=]() { m_ImGuiLayer->End(); });
				Renderer::EndFrame();

				// On Render thread
				m_Window->GetRenderContext()->BeginFrame();
				Renderer::WaitAndRender();
				m_Window->SwapBuffers();
			}

			float time = GetTime();
			m_Frametime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_Frametime, 0.333f);
			m_LastFrameTime = time;

			frameCounter++;

			LM_PROFILE_MARK_FRAME;
		}
		OnShutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnShutdown()
	{
		m_EventCallbacks.clear();
		g_ApplicationRunning = false;
	}

	void Application::ProcessEvents()
	{
		m_Window->ProcessEvents();

		// Note: we have no control over what func() does.  holding this lock while calling func() is a bad idea:
		// 1) func() might be slow (means we hold the lock for ages)
		// 2) func() might result in events getting queued, in which case we have a deadlock
		std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

		// Process custom event queue, up until we encounter an event that is not yet sync'd
		// If application queues such events, then it is the application's responsibility to call
		// SyncEvents() at the appropriate time.
		while (m_EventQueue.size() > 0)
		{
			const auto& [synced, func] = m_EventQueue.front();
			if (!synced)
			{
				break;
			}
			func();
			m_EventQueue.pop_front();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;

			if (event.Handled)
				return;

			// TODO: Should these callbacks be called BEFORE the layers recieve events?
			//				We may actually want that since most of these callbacks will be functions REQUIRED in order for the game
			//				to work, and if a layer has already handled the event we may end up with problems
			for (auto& eventCallback : m_EventCallbacks)
			{
				eventCallback(event);

				if (event.Handled)
					break;
			}
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		m_Window->GetRenderContext()->OnResize(width, height);

		auto& fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto& fb : fbs)
		{
			if (!fb->GetSpecification().NoResize)
				fb->Resize(width, height);
		}
		return false;
	}

	bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
	{
		m_Minimized = e.IsMinimized();
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	float Application::GetTime() const
	{
		return (float)Platform::GetTime();
	}

	const char* Application::GetConfigurationName()
	{
		return LM_BUILD_CONFIG_NAME;
	}

	const char* Application::GetPlatformName()
	{
		return LM_BUILD_PLATFORM_NAME;
	}

	std::thread::id Application::GetMainThreadID() { return s_MainThreadID; }

	bool Application::IsMainThread()
	{
		return std::this_thread::get_id() == s_MainThreadID;
	}

}