#include "lmpch.hpp"
#include "OpenGLIndexBuffer.hpp"

#include <glad/glad.h>

namespace Luma {

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
		: m_RendererID(0), m_Size(size)
	{
		LM_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
		});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		LM_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
		});
	}

	void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		LM_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		LM_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
		});
	}

}