#include "lmpch.hpp"
#include "Window.hpp"

#include "Luma/Platform/SDL/SDLWindow.hpp"

namespace Luma {

	Ref<Window> Window::Create(const WindowSpecification& specification)
	{
		switch (Current())
		{
			case WindowingAPI::SDL: return Ref<SDLWindow>::Create(specification);
			// case WindowingAPI::Win32: return Ref<Win32Window>::Create(specification);
			case WindowingAPI::None: return nullptr;
			default:
				LM_CORE_ASSERT(false, "Unsupported WindowingAPI");
				return nullptr;
		}
	}

}
