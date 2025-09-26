#pragma once

#include "Luma/Core/Base.hpp"
#include "Luma/Core/Window.hpp"

#include <string>

namespace Luma {

	struct ApplicationSpecification
	{
		std::string Name = "Luma";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
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
		virtual void OnUpdate() {}

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		static const char* GetConfigurationName();
		static const char* GetPlatformName();
	private:
		std::unique_ptr<Window> m_Window;
		ApplicationSpecification m_Specification;

		bool m_Running = true, m_Minimized = false;

		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}