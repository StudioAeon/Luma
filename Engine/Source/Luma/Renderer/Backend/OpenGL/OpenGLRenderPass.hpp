#pragma once

#include "Luma/Renderer/RenderPass.hpp"

namespace Luma {

	class OpenGLRenderPass : public RenderPass
	{
	public:
		OpenGLRenderPass(const RenderPassSpecification& spec);
		virtual ~OpenGLRenderPass();

		virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }
	private:
		RenderPassSpecification m_Specification;
	};

}