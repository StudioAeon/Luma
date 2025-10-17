#pragma once

#include "Luma/Asset/Asset.hpp"

#include "Luma/Asset/AssetManager.hpp"
#include "Luma/Renderer/Texture.hpp"

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Luma::UI {

	static int s_UIContextID = 0;

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	static void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}

	static bool Property(const char* label, std::string& value, bool error = false)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy(buffer, value.c_str());

		ImGui::PushID(label);
		if (error)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		if (ImGui::InputText("##value", buffer, 256))
		{
			value = buffer;
			modified = true;
		}
		if (error)
			ImGui::PopStyleColor();
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static void Property(const char* label, const char* value)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		ImGui::InputText("##value", (char*)value, 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	static bool Property(const char* label, bool& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::Checkbox("##value", &value))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, int& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::DragInt("##value", &value))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertySlider(const char* label, int& value, int min, int max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::SliderInt("##value", &value, min, max))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertySlider(const char* label, float& value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::SliderFloat("##value", &value, min, max))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertySlider(const char* label, glm::vec2& value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::SliderFloat2("##value", glm::value_ptr(value), min, max))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertySlider(const char* label, glm::vec3& value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::SliderFloat3("##value", glm::value_ptr(value), min, max))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertySlider(const char* label, glm::vec4& value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::SliderFloat4("##value", glm::value_ptr(value), min, max))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, bool readOnly = false)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (!readOnly)
		{
			if (ImGui::DragFloat("##value", &value, delta, min, max))
				modified = true;
		}
		else
		{
			ImGui::InputFloat("##value", &value, 0.0F, 0.0F, "%.3f", ImGuiInputTextFlags_ReadOnly);
		}
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, glm::vec2& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::DragFloat2("##value", glm::value_ptr(value), delta))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertyColor(const char* label, glm::vec3& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::ColorEdit3("##value", glm::value_ptr(value)))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, glm::vec3& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::DragFloat3("##value", glm::value_ptr(value), delta))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, glm::vec4& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		ImGui::PushID(label);
		if (ImGui::DragFloat4("##value", glm::value_ptr(value), delta))
			modified = true;
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected)
	{
		const char* current = options[*selected];
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		bool changed = false;

		std::string id = "##" + std::string(label);
		if (ImGui::BeginCombo(id.c_str(), current))
		{
			for (int i = 0; i < optionCount; i++)
			{
				bool is_selected = (current == options[i]);
				if (ImGui::Selectable(options[i], is_selected))
				{
					current = options[i];
					*selected = i;
					changed = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return changed;
	}

	static bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected)
	{
		const char* current = options[*selected].c_str();
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		bool changed = false;

		std::string id = "##" + std::string(label);
		if (ImGui::BeginCombo(id.c_str(), current))
		{
			for (int i = 0; i < optionCount; i++)
			{
				bool is_selected = (current == options[i]);
				if (ImGui::Selectable(options[i].c_str(), is_selected))
				{
					current = options[i].c_str();
					*selected = i;
					changed = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return changed;
	}

	template<typename T>
	static bool PropertyAssetReference(const char* label, Ref<T>& object, AssetType supportedType)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (object)
		{
			if (object->Type != AssetType::Missing)
			{
				char* assetName = ((Ref<Asset>&)object)->FileName.data();
				ImGui::InputText("##assetRef", assetName, 256, ImGuiInputTextFlags_ReadOnly);
			}
			else
			{
				char buffer[256] = "Missing";
				ImGui::InputText("##assetRef", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
			}
		}
		else
		{
			ImGui::InputText("##assetRef", (char*)"Null", 256, ImGuiInputTextFlags_ReadOnly);
		}

		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("asset_payload");

			if (data)
			{
				AssetHandle assetHandle = *(AssetHandle*)data->Data;
				Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
				if (asset->Type == supportedType)
				{
					object = asset.As<T>();
					modified = true;
				}
			}
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return modified;
	}

	static void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}

	static bool BeginTreeNode(const char* name, bool defaultOpen = true)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (defaultOpen)
			treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(name, treeNodeFlags);
	}

	static void EndTreeNode()
	{
		ImGui::TreePop();
	}

	static int s_CheckboxCount = 0;
	static int s_CheckboxGroupCounter = 0;

	static void BeginCheckboxGroup(const char* label)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		ImGui::PushID(label);
		s_CheckboxGroupCounter = 0;
	}

	static bool PropertyCheckboxGroup(const char* label, bool& value)
	{
		bool modified = false;

		if (++s_CheckboxCount > 1)
			ImGui::SameLine();

		ImGui::Text(label);
		ImGui::SameLine();

		ImGui::PushID(s_CheckboxGroupCounter++);
		if (ImGui::Checkbox("##value", &value))
			modified = true;
		ImGui::PopID();

		return modified;
	}

	static void EndCheckboxGroup()
	{
		ImGui::PopID();
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		s_CheckboxCount = 0;
	}

	void Image(const Ref<Image2D>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	void Image(const Ref<Image2D>& image, uint32_t layer, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	void Image(const Ref<Texture2D>& texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	bool ImageButton(const Ref<Image2D>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool ImageButton(const Ref<Texture2D>& texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

}