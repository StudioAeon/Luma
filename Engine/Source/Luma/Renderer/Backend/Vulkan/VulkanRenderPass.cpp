#include "lmpch.hpp"
#include "VulkanRenderPass.hpp"

namespace Luma {

	VulkanRenderPass::VulkanRenderPass(const RenderPassSpecification& spec)
		: m_Specification(spec)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

}