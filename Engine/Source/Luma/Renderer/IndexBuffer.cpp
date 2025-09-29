#include "lmpch.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLIndexBuffer.hpp"

namespace Luma {

	IndexBuffer* IndexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return new OpenGLIndexBuffer(size);
		}
		return nullptr;

	}

}