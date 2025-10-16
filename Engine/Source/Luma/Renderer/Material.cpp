#include "lmpch.hpp"
#include "Material.hpp"

#include "Luma/Renderer/Backend/Vulkan/VulkanMaterial.hpp"
#include "Luma/Renderer/Backend/OpenGL/OpenGLMaterial.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

namespace Luma {

	Ref<Material> Material::Create(const Ref<Shader>& shader, const std::string& name)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::Vulkan: return Ref<VulkanMaterial>::Create(shader, name);
			case RendererAPIType::OpenGL: return Ref<OpenGLMaterial>::Create(shader, name);
		}
		LM_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}