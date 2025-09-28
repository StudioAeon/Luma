#pragma once

#include "Luma/Core/Window.hpp"

#include "SDL3/SDL.h"

namespace Luma {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& specification);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;
	private:
		virtual void Init();
		virtual void Shutdown();
	private:
		SDL_Window* m_Window = nullptr;
		SDL_WindowID m_WindowID = 0;
		SDL_Event m_Event{};

		WindowSpecification m_Specification;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		}; WindowData m_Data;
	};

}