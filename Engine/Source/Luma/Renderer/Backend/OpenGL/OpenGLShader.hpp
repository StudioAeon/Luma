#pragma once

#include "Luma/Renderer/Shader.hpp"

namespace Luma {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);

		virtual void Bind() override;
	private:
		void ReadShaderFromFile(const std::string& filepath);
		void CompileAndUploadShader();
	private:
		RendererID m_RendererID;

		std::string m_ShaderSource;
	};

}