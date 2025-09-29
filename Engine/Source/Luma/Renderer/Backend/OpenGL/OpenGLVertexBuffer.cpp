#include "lmpch.hpp"
#include "OpenGLVertexBuffer.hpp"

#include <glad/glad.h>

namespace Luma {

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		: m_RendererID(0), m_Size(size)
	{
		LM_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		LM_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
		});
	}

	void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		LM_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		});

	}

	void OpenGLVertexBuffer::Bind() const
	{
		LM_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
		});
	}

}