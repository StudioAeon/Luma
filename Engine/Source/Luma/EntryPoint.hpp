#pragma once

#include "Luma/Core/Application.hpp"
#include "Luma/Core/Assert.hpp"

extern Luma::Application* Luma::CreateApplication(int argc, char** argv);
extern bool g_ApplicationRunning;

namespace Luma {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			InitializeCore();

			Application* app = CreateApplication(argc, argv);
			LM_CORE_ASSERT(app, "Client Application is null!");

			app->Run();

			delete app;
			ShutdownCore();
		}
		return 0;
	}

}

#if defined(ZN_PLATFORM_WINDOWS)
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	return Luma::Main(__argc, __argv);
}
#else
int main(int argc, char** argv)
{
	return Luma::Main(argc, argv);
}
#endif