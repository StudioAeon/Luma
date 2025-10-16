#include "lmpch.hpp"
#include "RendererContext.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLContext.hpp"
#include "Luma/Renderer/Backend/Vulkan/VulkanContext.hpp"

namespace Luma {

	Ref<RendererContext> RendererContext::Create(SDL_Window* windowHandle)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLContext>::Create(windowHandle);
			case RendererAPIType::Vulkan:  return Ref<VulkanContext>::Create(windowHandle);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}