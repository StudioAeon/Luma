#pragma once

#include "Luma/Core/Window.hpp"

namespace Luma {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallbackFn = callback; }

		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }
	protected:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		std::string m_Title;
		uint32_t m_Width, m_Height;

		EventCallbackFn m_EventCallbackFn;
	};

}