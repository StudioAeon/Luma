#include "lmpch.hpp"
#include "RenderPass.hpp"

#include "Renderer.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLRenderPass.hpp"

namespace Luma {

	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    LM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLRenderPass>::Create(spec);
		}

		LM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}