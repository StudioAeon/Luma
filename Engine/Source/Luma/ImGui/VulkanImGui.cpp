#include "lmpch.hpp"
#include "ImGui.hpp"

#include "Luma/Renderer/RendererAPI.hpp"

#include "Luma/Renderer/Backend/Vulkan/VulkanTexture.hpp"
#include "Luma/Renderer/Backend/Vulkan/VulkanImage.hpp"

#include "Luma/Renderer/Backend/OpenGL/OpenGLTexture.hpp"
#include "Luma/Renderer/Backend/OpenGL/OpenGLImage.hpp"

#include <backends/imgui_impl_vulkan.h>

#include <unordered_map>

namespace Luma::UI {

	// ========================================================================
	// Internal Cache Implementation
	// ========================================================================

	namespace Internal {

		struct VulkanDescriptorCache
		{
			std::unordered_map<uint64_t, ImTextureID> Cache;

			ImTextureID GetOrCreate(uint64_t hash,
								   VkSampler sampler,
								   VkImageView imageView,
								   VkImageLayout layout)
			{
				// Check cache first
				auto it = Cache.find(hash);
				if (it != Cache.end()) {
					return it->second;
				}

				// Create new descriptor set
				VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(sampler, imageView, layout);
				if (!descriptorSet) {
					LM_CORE_ERROR("Failed to create ImGui Vulkan texture descriptor");
					return (ImTextureID)0;
				}

				// Cache it (cast VkDescriptorSet to ImTextureID)
				ImTextureID textureID = (ImTextureID)(uintptr_t)descriptorSet;
				Cache[hash] = textureID;
				LM_CORE_TRACE("Cached ImGui descriptor for hash {0}", hash);
				return textureID;
			}

			void Clear()
			{
				// Remove all descriptor sets from ImGui
				for (auto& [hash, textureID] : Cache) {
					ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)(uintptr_t)textureID);
				}

				size_t count = Cache.size();
				Cache.clear();
				if (count > 0) {
					LM_CORE_INFO("Cleared ImGui descriptor cache ({0} entries)", count);
				}
			}

			void Remove(uint64_t hash)
			{
				auto it = Cache.find(hash);
				if (it != Cache.end()) {
					ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)(uintptr_t)it->second);
					Cache.erase(it);
					LM_CORE_TRACE("Removed descriptor from cache for hash {0}", hash);
				}
			}
		};

		static VulkanDescriptorCache s_DescriptorCache;

		VulkanDescriptorCache& GetDescriptorCache()
		{
			return s_DescriptorCache;
		}

	} // namespace Internal

	// ========================================================================
	// Public Cache Management
	// ========================================================================

	void ClearDescriptorCache()
	{
		if (RendererAPI::Current() != RendererAPIType::Vulkan)
			return;

		Internal::GetDescriptorCache().Clear();
	}

	void RemoveFromCache(uint64_t imageHash)
	{
		if (RendererAPI::Current() != RendererAPIType::Vulkan)
			return;

		Internal::GetDescriptorCache().Remove(imageHash);
	}

	// ========================================================================
	// Image Functions
	// ========================================================================

	void Image(const Ref<Image2D>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
			   const ImVec4& tint_col, const ImVec4& border_col)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			Ref<OpenGLImage2D> glImage = image.As<OpenGLImage2D>();
			ImGui::Image((ImTextureID)(uint64_t)glImage->GetRendererID(), size, uv0, uv1, tint_col, border_col);
		}
		else
		{
			Ref<VulkanImage2D> vulkanImage = image.As<VulkanImage2D>();
			const auto& imageInfo = vulkanImage->GetImageInfo();

			if (!imageInfo.ImageView) {
				LM_CORE_WARN("Attempting to render Image2D with null ImageView");
				return;
			}

			// Use image hash for caching
			uint64_t hash = vulkanImage->GetHash();
			const auto& descriptor = vulkanImage->GetDescriptor();

			auto& cache = Internal::GetDescriptorCache();
			ImTextureID textureID = cache.GetOrCreate(hash, imageInfo.Sampler, imageInfo.ImageView, descriptor.imageLayout);

			if (textureID) {
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}
	}

	void Image(const Ref<Image2D>& image, uint32_t imageLayer, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			Ref<OpenGLImage2D> glImage = image.As<OpenGLImage2D>();
			ImGui::Image((ImTextureID)(uint64_t)glImage->GetRendererID(), size, uv0, uv1, tint_col, border_col);
		}
		else
		{
			Ref<VulkanImage2D> vulkanImage = image.As<VulkanImage2D>();
			VkImageView layerImageView = vulkanImage->GetLayerImageView(imageLayer);

			if (!layerImageView) {
				LM_CORE_WARN("Attempting to render Image2D layer with null ImageView");
				return;
			}

			const auto& imageInfo = vulkanImage->GetImageInfo();
			const auto& descriptor = vulkanImage->GetDescriptor();

			// Create unique hash combining image hash and layer index
			uint64_t hash = vulkanImage->GetHash() ^ (static_cast<uint64_t>(imageLayer) << 32);

			auto& cache = Internal::GetDescriptorCache();
			ImTextureID textureID = cache.GetOrCreate(hash, imageInfo.Sampler, layerImageView, descriptor.imageLayout);

			if (textureID) {
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}
	}

	void Image(const Ref<Texture2D>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
			   const ImVec4& tint_col, const ImVec4& border_col)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			Ref<OpenGLImage2D> image = texture->GetImage().As<OpenGLImage2D>();
			ImGui::Image((ImTextureID)(uint64_t)image->GetRendererID(), size, uv0, uv1, tint_col, border_col);
		}
		else
		{
			Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
			const VkDescriptorImageInfo& imageInfo = vulkanTexture->GetVulkanDescriptorInfo();

			if (!imageInfo.imageView) {
				LM_CORE_WARN("Attempting to render Texture2D with null imageView");
				return;
			}

			// Use texture hash for caching
			uint64_t hash = vulkanTexture->GetHash();

			auto& cache = Internal::GetDescriptorCache();
			ImTextureID textureID = cache.GetOrCreate(hash, imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);

			if (textureID) {
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}
	}

	// ========================================================================
	// ImageButton Functions
	// ========================================================================

	bool ImageButton(const Ref<Image2D>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
					 int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			Ref<OpenGLImage2D> glImage = image.As<OpenGLImage2D>();
			ImGui::PushStyleColor(ImGuiCol_Button, bg_col);
			bool result = ImGui::ImageButton("##image_button", (ImTextureID)(uint64_t)glImage->GetRendererID(), size, uv0, uv1, tint_col);
			ImGui::PopStyleColor();
			return result;
		}
		else
		{
			Ref<VulkanImage2D> vulkanImage = image.As<VulkanImage2D>();
			const auto& imageInfo = vulkanImage->GetImageInfo();

			if (!imageInfo.ImageView) {
				LM_CORE_WARN("Attempting to render ImageButton with null ImageView");
				return false;
			}

			uint64_t hash = vulkanImage->GetHash();
			const auto& descriptor = vulkanImage->GetDescriptor();

			auto& cache = Internal::GetDescriptorCache();
			ImTextureID textureID = cache.GetOrCreate(hash, imageInfo.Sampler, imageInfo.ImageView, descriptor.imageLayout);

			if (!textureID)
				return false;

			// Use hash as unique ID to avoid collisions
			char buttonID[32];
			snprintf(buttonID, sizeof(buttonID), "##imgbtn_%llu", hash);

			ImGui::PushStyleColor(ImGuiCol_Button, bg_col);
			bool result = ImGui::ImageButton(buttonID, textureID, size, uv0, uv1, tint_col);
			ImGui::PopStyleColor();
			return result;
		}
	}

	bool ImageButton(const Ref<Texture2D>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
					 int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			Ref<OpenGLImage2D> image = texture->GetImage().As<OpenGLImage2D>();
			ImGui::PushStyleColor(ImGuiCol_Button, bg_col);
			bool result = ImGui::ImageButton("##image_button", (ImTextureID)(uint64_t)image->GetRendererID(), size, uv0, uv1, tint_col);
			ImGui::PopStyleColor();
			return result;
		}
		else
		{
			Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
			const VkDescriptorImageInfo& imageInfo = vulkanTexture->GetVulkanDescriptorInfo();

			if (!imageInfo.imageView) {
				LM_CORE_WARN("Attempting to render ImageButton with null imageView");
				return false;
			}

			uint64_t hash = vulkanTexture->GetHash();

			auto& cache = Internal::GetDescriptorCache();
			ImTextureID textureID = cache.GetOrCreate(hash, imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);

			if (!textureID)
				return false;

			// Use hash as unique ID
			char buttonID[32];
			snprintf(buttonID, sizeof(buttonID), "##imgbtn_%llu", hash);

			ImGui::PushStyleColor(ImGuiCol_Button, bg_col);
			bool result = ImGui::ImageButton(buttonID, textureID, size, uv0, uv1, tint_col);
			ImGui::PopStyleColor();
			return result;
		}
	}

} // namespace Luma::UI