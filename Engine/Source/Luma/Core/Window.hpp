#pragma once

#include <functional>

#include "Luma/Core/Base.hpp"
#include "Luma/Events/Event.hpp"

namespace Luma {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Luma Engine",
					uint32_t width = 1600,
					uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window() {}

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	protected:
		virtual void Init(const WindowProps& props) = 0;
		virtual void Shutdown() = 0;
	};

}