#include "lmpch.hpp"
#include "VertexArray.hpp"

#include "Renderer.hpp"
#include "Luma/Renderer/Backend/OpenGL/OpenGLVertexArray.hpp"

namespace Luma {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    LM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLVertexArray>::Create();
		}

		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}