#include "lmpch.hpp"
#include "OpenGLImage.hpp"

#include "Luma/Renderer/Renderer.hpp"

namespace Luma {

	OpenGLImage2D::OpenGLImage2D(ImageSpecification specification, const void* data)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		// TODO: Local storage should be optional
		if (data)
			m_ImageData = Buffer::Copy(data, Utils::GetImageMemorySize(specification.Format, specification.Width, specification.Height));
	}

	OpenGLImage2D::OpenGLImage2D(ImageSpecification specification, Buffer buffer)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height), m_ImageData(buffer)
	{
	}

	OpenGLImage2D::~OpenGLImage2D()
	{
		// Should this be submitted?
		m_ImageData.Release();
		if (m_RendererID)
		{
			RendererID rendererID = m_RendererID;
			Renderer::Submit([rendererID]()
			{
				glDeleteTextures(1, &rendererID);
			});
		}
	}

	void OpenGLImage2D::Invalidate()
	{
		if (m_RendererID)
			Release();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		GLenum internalFormat = Utils::OpenGLImageInternalFormat(m_Specification.Format);
		uint32_t mipCount = Utils::CalculateMipCount(m_Width, m_Height);
		glTextureStorage2D(m_RendererID, mipCount, internalFormat, m_Width, m_Height);
		if (m_ImageData)
		{
			GLenum format = Utils::OpenGLImageFormat(m_Specification.Format);
			GLenum dataType = Utils::OpenGLFormatDataType(m_Specification.Format);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, dataType, m_ImageData.Data);
			glGenerateTextureMipmap(m_RendererID); // TODO: optional
		}
	}

	void OpenGLImage2D::Release()
	{
		if (m_RendererID)
		{
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;
		}
		m_ImageData.Release();
	}

	void OpenGLImage2D::CreateSampler(TextureProperties properties)
	{
		glCreateSamplers(1, &m_SamplerRendererID);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
	}

}