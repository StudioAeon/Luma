#pragma once

#include "Luma/Core/Base.hpp"
#include "Luma/Events/Event.hpp"

#include "Luma/Renderer/RendererContext.hpp"

#include <SDL3/SDL.h>

#include <functional>
#include <filesystem>

namespace Luma {

	enum class WindowMode
	{
		Windowed,
		BorderlessFullscreen,
		ExclusiveFullscreen
	};

	struct WindowSpecification
	{
		std::string Title = "Luma";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		WindowMode Mode = WindowMode::Windowed;
		bool VSync = true;
		std::filesystem::path IconPath;
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void PollEvents() = 0;
		virtual void ProcessEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float, float> GetWindowPos() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetResizable(bool resizable) const = 0;

		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual Ref<RendererContext> GetRenderContext() = 0;
	public:
		static Ref<Window> Create(const WindowSpecification& specification = WindowSpecification());
	};

}