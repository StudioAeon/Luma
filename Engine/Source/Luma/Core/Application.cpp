#include "lmpch.hpp"
#include "Application.hpp"

bool g_ApplicationRunning = true;
namespace Luma {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Instance = this;
	}

	Application::~Application()
	{}

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
		{}
		OnShutdown();
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