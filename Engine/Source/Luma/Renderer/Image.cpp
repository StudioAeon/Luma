#include "lmpch.hpp"
#include "Image.hpp"

#include "Luma/Renderer/Backend/Vulkan/VulkanImage.hpp"
#include "Luma/Renderer/Backend/OpenGL/OpenGLImage.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

namespace Luma {

	Ref<Image2D> Image2D::Create(ImageSpecification specification, Buffer buffer)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLImage2D>::Create(specification, buffer);
			case RendererAPIType::Vulkan: return Ref<VulkanImage2D>::Create(specification);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Image2D> Image2D::Create(ImageSpecification specification, const void* data)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLImage2D>::Create(specification, data);
			case RendererAPIType::Vulkan: return Ref<VulkanImage2D>::Create(specification);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}