#include "lmpch.hpp"
#include "WindowsWindow.hpp"

namespace Luma {

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
		: Window(props)
	{}

	WindowsWindow::~WindowsWindow()
	{}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Title = props.Title;
		m_Width = props.Width;
		m_Height = props.Height;

		// TODO: Platform Specific
	}

	void WindowsWindow::Shutdown()
	{}

}