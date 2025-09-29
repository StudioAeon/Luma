#include "lmpch.hpp"
#include "Application.hpp"

#include "Luma/Renderer/Renderer.hpp"
#include <glad/glad.h>

bool g_ApplicationRunning = true;
namespace Luma {

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Instance = this;

		WindowSpecification windowSpec;
		windowSpec.Title = specification.Name;
		windowSpec.Width = specification.WindowWidth;
		windowSpec.Height = specification.WindowHeight;
		windowSpec.VSync = specification.VSync;
		m_Window = std::unique_ptr<Window>(Window::Create(windowSpec));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		Renderer::Init();
	}

	Application::~Application()
	{}

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
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnShutdown()
	{
		g_ApplicationRunning = false;
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				// Render ImGui on render thread
				Application* app = this;
				LM_RENDER_1(app, { app->RenderImGui(); });
			}

			Renderer::Get().WaitAndRender();

			m_Window->OnUpdate();
		}
		OnShutdown();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//LM_CORE_TRACE("{}", event.ToString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	const char* Application::GetConfigurationName()
	{
		return LM_BUILD_CONFIG_NAME;
	}

	const char* Application::GetPlatformName()
	{
		return LM_BUILD_PLATFORM_NAME;
	}

}