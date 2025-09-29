#include "lmpch.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLVertexBuffer.hpp"

namespace Luma {

	VertexBuffer* VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return new OpenGLVertexBuffer(size);
		}
		return nullptr;

	}

}