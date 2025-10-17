#include "lmpch.hpp"
#include "UniformBuffer.hpp"

#include "Luma/Renderer/Renderer.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLUniformBuffer.hpp"
#include "Luma/Renderer/Backend/Vulkan/VulkanUniformBuffer.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

namespace Luma {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:     return nullptr;
			case RendererAPIType::Vulkan:  return Ref<VulkanUniformBuffer>::Create(size, binding);
			case RendererAPIType::OpenGL:  return Ref<OpenGLUniformBuffer>::Create(size, binding);
		}

		LM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}