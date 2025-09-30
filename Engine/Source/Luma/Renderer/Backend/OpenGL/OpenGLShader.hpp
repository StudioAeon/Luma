#pragma once

#include "Luma/Renderer/Shader.hpp"

namespace Luma {

	class OpenGLShader : public Shader
	{
	public:
		enum class ShaderType
		{
			None       = 0,
			Vertex     = 1,

			// Fragment and Pixel shaders are the same
			Fragment   = 2,
			Pixel      = 2
		};
	public:
		OpenGLShader(const std::string& filepath);

		virtual void Bind() override;
	private:
		void ReadShaderFromFile(const std::string& filepath);
		void CompileAndUploadShader();

		static ShaderType ShaderTypeFromString(const std::string& type);
	private:
		RendererID m_RendererID;

		std::string m_ShaderSource;
	};

}