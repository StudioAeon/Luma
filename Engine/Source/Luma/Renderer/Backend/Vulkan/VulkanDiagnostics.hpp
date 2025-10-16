#pragma once

#include "Vulkan.hpp"

namespace Luma::Utils {

	struct VulkanCheckpointData
	{
		char Data[64];
	};

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const std::string& data);

}

