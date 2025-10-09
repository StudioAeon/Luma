#include "lmpch.hpp"
#include "IndexBuffer.hpp"

#include "Renderer.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLIndexBuffer.hpp"

namespace Luma {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(size);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(data, size);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}