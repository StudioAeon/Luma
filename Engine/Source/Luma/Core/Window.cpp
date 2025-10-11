#include "lmpch.hpp"
#include "Window.hpp"

#include "Luma/Events/ApplicationEvent.hpp"
#include "Luma/Events/KeyEvent.hpp"
#include "Luma/Events/MouseEvent.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>

#include <glad/glad.h>

namespace Luma {

	static void SDLErrorCallback(const char* context)
	{
		LM_CORE_ERROR("SDL Error ({}): {}", context, SDL_GetError());
	}

	static bool s_SDLInitialized = false;

	Window* Window::Create(const WindowSpecification& specification)
	{
		return new Window(specification);
	}

	Window::Window(const WindowSpecification& specification)
		: m_Specification(specification)
	{}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init()
	{
		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		LM_CORE_INFO("Creating window {} ({}x{})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_SDLInitialized)
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
			{
				SDLErrorCallback("SDL_Init");
				LM_CORE_ASSERT(false, "Could not initialize SDL!");
			}
			s_SDLInitialized = true;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY;

		switch (m_Specification.Mode)
		{
			case WindowMode::Windowed:
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags);
				break;
			case WindowMode::BorderlessFullscreen:
				windowFlags |= SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN;
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), 0, 0, windowFlags);
				break;
			case WindowMode::ExclusiveFullscreen:
			{
				SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
				const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), mode->w, mode->h, windowFlags | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MOUSE_GRABBED);
				break;
			}
		}

		m_GLContext = SDL_GL_CreateContext(m_Window);
		SDL_GL_MakeCurrent(m_Window, m_GLContext);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
		LM_CORE_ASSERT(status, "Failed to initialize Glad!");

		SDL_SetPointerProperty(SDL_GetWindowProperties(m_Window), "WindowData", &m_Data);

		m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
		m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_MOVE);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NS_RESIZE);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_EW_RESIZE);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NESW_RESIZE);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE);
		m_ImGuiMouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

		// Update window size to actual size
		int w, h;
		SDL_GetWindowSize(m_Window, &w, &h);
		m_Data.Width = w;
		m_Data.Height = h;
	}

	void Window::Shutdown()
	{
		if (m_Window) {
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
		SDL_Quit();
		s_SDLInitialized = false;
	}

	std::pair<float, float> Window::GetWindowPos() const
	{
		int x, y;
		SDL_GetWindowPosition(m_Window, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	void Window::ProcessEvents()
	{
		PollEvents();
	}

	void Window::PollEvents()
	{
		while (SDL_PollEvent(&m_Event))
		{
			// Let ImGui handle events first
			if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr)
			{
				ImGui_ImplSDL3_ProcessEvent(&m_Event);
			}

			switch (m_Event.type)
			{
				case SDL_EVENT_QUIT:
				{
					WindowCloseEvent e;
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_WINDOW_RESIZED:
				{
					WindowResizeEvent e(static_cast<uint32_t>(m_Event.window.data1), static_cast<uint32_t>(m_Event.window.data2));
					m_Data.Width = m_Event.window.data1;
					m_Data.Height = m_Event.window.data2;
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_WINDOW_MINIMIZED:
				{
					WindowMinimizeEvent e(true);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_WINDOW_RESTORED:
				{
					WindowMinimizeEvent e(false);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					SDL_Scancode scancode = m_Event.key.scancode;
					int repeatCount = m_Event.key.repeat ? 1 : 0;

					KeyPressedEvent e(static_cast<KeyCode>(scancode), repeatCount);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_KEY_UP:
				{
					SDL_Scancode scancode = m_Event.key.scancode;

					KeyReleasedEvent e(static_cast<KeyCode>(scancode));
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_TEXT_INPUT:
				{
					// SDL3 text input is a string, take first character
					KeyTypedEvent e(static_cast<KeyCode>(m_Event.text.text[0]));
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					MouseButtonPressedEvent e(m_Event.button.button);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					MouseButtonReleasedEvent e(m_Event.button.button);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					MouseMovedEvent e(m_Event.motion.x, m_Event.motion.y);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_WHEEL:
				{
					MouseScrolledEvent e(m_Event.wheel.x, m_Event.wheel.y);
					if (m_Data.EventCallback)
						m_Data.EventCallback(e);
					break;
				}
			}
		}
	}

	void Window::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_Window);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);
		m_Specification.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Specification.VSync;
	}

	void Window::SetResizable(bool resizable) const
	{
		SDL_SetWindowResizable(m_Window, resizable);
	}

	void Window::Maximize()
	{
		if (m_Specification.Mode == WindowMode::Windowed)
			SDL_MaximizeWindow(m_Window);
	}

	void Window::CenterWindow()
	{
		SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* videomode = SDL_GetCurrentDisplayMode(displayID);
		int x = (videomode->w / 2) - (m_Data.Width / 2);
		int y = (videomode->h / 2) - (m_Data.Height / 2);
		SDL_SetWindowPosition(m_Window, x, y);
	}

	void Window::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		SDL_SetWindowTitle(m_Window, title.c_str());
	}

}
