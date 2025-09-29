#include "lmpch.hpp"
#include "Shader.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLShader.hpp"

namespace Luma {

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return new OpenGLShader(filepath);
		}
		return nullptr;
	}

}