#include "lmpch.hpp"
#include "RendererContext.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLContext.hpp"

namespace Luma {

	Ref<RendererContext> RendererContext::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLContext>::Create();
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}