#pragma once

#include "Luma/Core/Base.hpp"
#include "Luma/Core/TimeStep.hpp"
#include "Luma/Core/Window.hpp"
#include "Luma/Core/LayerStack.hpp"

#include "Luma/Events/ApplicationEvent.hpp"

#include "Luma/ImGui/ImGuiLayer.hpp"

#include <string>

namespace Luma {

	struct ApplicationSpecification
	{
		std::string Name = "Luma";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		bool VSync = true;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void Close();

		virtual void OnInit() {}
		virtual void OnShutdown();
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		void RenderImGui();

		std::string OpenFile(const std::string& filter) const;

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		Timestep GetTimestep() const { return m_TimeStep; }
		Timestep GetFrametime() const { return m_Frametime; }
		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		float GetFrameDelta(); // TODO: This should be in "Platform"

		static const char* GetConfigurationName();
		static const char* GetPlatformName();
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ApplicationSpecification m_Specification;

		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		Timestep m_Frametime;
		Timestep m_TimeStep;

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}