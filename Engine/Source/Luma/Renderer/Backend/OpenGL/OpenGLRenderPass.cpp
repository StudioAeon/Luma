#include "lmpch.hpp"
#include "OpenGLRenderPass.hpp"

namespace Luma {

	OpenGLRenderPass::OpenGLRenderPass(const RenderPassSpecification& spec)
		: m_Specification(spec)
	{}

	OpenGLRenderPass::~OpenGLRenderPass()
	{}

}