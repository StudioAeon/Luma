#include "lmpch.hpp"
#include "VertexBuffer.hpp"

#include "Renderer.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLVertexBuffer.hpp"

namespace Luma {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(data, size, usage);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(size, usage);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}