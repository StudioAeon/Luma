#include "lmpch.hpp"
#include "Texture.hpp"

#include "Luma/Renderer/RendererAPI.hpp"
#include "Luma/Renderer/Backend/OpenGL/OpenGLTexture.hpp"
#include "Luma/Renderer/Backend/Vulkan/VulkanTexture.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

namespace Luma {

	Ref<Texture2D> Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(format, width, height, data);
			case RendererAPIType::Vulkan: return Ref<VulkanTexture2D>::Create(format, width, height, data);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(path, srgb);
			case RendererAPIType::Vulkan: return Ref<VulkanTexture2D>::Create(path, srgb);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLTextureCube>::Create(format, width, height, data);
			case RendererAPIType::Vulkan: return Ref<VulkanTextureCube>::Create(format, width, height, data);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(const std::string& path)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return Ref<OpenGLTextureCube>::Create(path);
				case RendererAPIType::Vulkan: return Ref<VulkanTextureCube>::Create(path);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}





}