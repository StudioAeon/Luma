#include "lmpch.hpp"
#include "Pipeline.hpp"

#include "Renderer.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLPipeline.hpp"

namespace Luma {

	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLPipeline>::Create(spec);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}