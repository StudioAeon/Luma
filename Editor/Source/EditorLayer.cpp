#include "EditorLayer.hpp"

#include <iostream>

namespace Luma {

	static void ImGuiShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	EditorLayer::EditorLayer()
		: m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f}
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::OnAttach()
	{
		LM_INFO("Hello from app!");
	}

	void EditorLayer::OnDetach()
	{}

	void EditorLayer::OnUpdate()
	{
		Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Begin("EditorLayer");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{}

}