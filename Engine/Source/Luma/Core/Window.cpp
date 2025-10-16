#include "lmpch.hpp"
#include "Window.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

#include "Luma/Platform/SDL/SDLWindow.hpp"

namespace Luma {

	Ref<Window> Window::Create(const WindowSpecification& specification)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::OpenGL: return Ref<SDLWindow>::Create(specification);
			case RendererAPIType::Vulkan: return Ref<SDLWindow>::Create(specification);
		}
	}

}
