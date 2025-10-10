#pragma once

#include <functional>

#include "Luma/Core/Base.hpp"
#include "Luma/Events/Event.hpp"

namespace Luma {

	struct WindowSpecification
	{
		std::string Title = "Luma";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool VSync = true;
	};

	// Interface representing a desktop system based Window
	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float, float> GetWindowPos() const = 0;

		virtual void Maximize() = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& specification = WindowSpecification());
	};

}