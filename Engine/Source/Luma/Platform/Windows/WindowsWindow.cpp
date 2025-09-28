#include "lmpch.hpp"
#include "WindowsWindow.hpp"

#include "Luma/Events/ApplicationEvent.hpp"
#include "Luma/Events/KeyEvent.hpp"
#include "Luma/Events/MouseEvent.hpp"

namespace Luma {

	static bool s_SDLInitialized = false;

	Window* Window::Create(const WindowSpecification& specification)
	{
		return new WindowsWindow(specification);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& props)
		: m_Specification(props)
	{
		Init();
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		LM_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		if (!s_SDLInitialized)
		{
			if (!SDL_Init(SDL_INIT_VIDEO))
			{
				LM_CORE_ERROR("Could not initialize SDL3: {0}", SDL_GetError());
				LM_CORE_ASSERT(false, "Could not initialize SDL3!");
				return;
			}
			s_SDLInitialized = true;
		}

		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), (int)m_Specification.Width, (int)m_Specification.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		if (!m_Window)
		{
			LM_CORE_ERROR("Could not create SDL window: {0}", SDL_GetError());
			LM_CORE_ASSERT(false, "Could not create SDL window!");
			return;
		}

		m_WindowID = SDL_GetWindowID(m_Window);

		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		if (m_Window)
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}

		// Note: SDL_Quit() is typically called once at application shutdown
		// not per window destruction in case multiple windows exist
	}

	void WindowsWindow::OnUpdate()
	{
		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
				case SDL_EVENT_QUIT:
				{
					WindowCloseEvent event;
					m_Data.EventCallback(event);
					break;
				}
				case SDL_EVENT_WINDOW_RESIZED:
				{
					if (m_Event.window.windowID == m_WindowID)
					{
						int width, height;
						SDL_GetWindowSize(m_Window, &width, &height);

						WindowResizeEvent event((uint32_t)width, (uint32_t)height);
						m_Data.EventCallback(event);
						m_Data.Width = width;
						m_Data.Height = height;
					}
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					if (m_Event.key.windowID == m_WindowID)
					{
						int repeatCount = m_Event.key.repeat ? 1 : 0;
						KeyPressedEvent event((int)m_Event.key.scancode, repeatCount);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_KEY_UP:
				{
					if (m_Event.key.windowID == m_WindowID)
					{
						KeyReleasedEvent event((int)m_Event.key.scancode);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					if (m_Event.button.windowID == m_WindowID)
					{
						MouseButtonPressedEvent event((int)m_Event.button.button);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					if (m_Event.button.windowID == m_WindowID)
					{
						MouseButtonReleasedEvent event((int)m_Event.button.button);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_WHEEL:
				{
					if (m_Event.wheel.windowID == m_WindowID)
					{
						MouseScrolledEvent event((float)m_Event.wheel.x, (float)m_Event.wheel.y);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					if (m_Event.motion.windowID == m_WindowID)
					{
						MouseMovedEvent event((float)m_Event.motion.x, (float)m_Event.motion.y);
						m_Data.EventCallback(event);
					}
					break;
				}
			}
		}

		// SDL_GL_SwapWindow(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		// SDL3 VSync is controlled through the renderer or graphics context
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}