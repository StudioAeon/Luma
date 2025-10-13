#pragma once

 #include "Luma/Events/Event.hpp"
#include "Luma/Core/Window.hpp"

#include <SDL3/SDL.h>

#include <functional>

namespace Luma {

	class SDLWindow : public Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		SDLWindow(const WindowSpecification& specification);
		virtual ~SDLWindow() = default;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void PollEvents() override;
		virtual void ProcessEvents() override;
		virtual void SwapBuffers() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		virtual std::pair<uint32_t, uint32_t> GetSize() const override { return { m_Data.Width, m_Data.Height }; }
		virtual std::pair<float, float> GetWindowPos() const override;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		virtual void SetResizable(bool resizable) const override;

		virtual void Maximize() override;
		virtual void CenterWindow() override;

		virtual const std::string& GetTitle() const override { return m_Data.Title; }
		virtual void SetTitle(const std::string& title) override;

		SDL_Window* GetNativeWindow() const { return m_Window; }
	private:
		SDL_Window* m_Window = nullptr;
		SDL_GLContext m_GLContext;
		SDL_Cursor* m_ImGuiMouseCursors[8] = { nullptr };
		SDL_Event m_Event {};

		WindowSpecification m_Specification;
		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;

			EventCallbackFn EventCallback;
		}; WindowData m_Data;

		float m_LastFrameTime = 0.0f;
	};

}