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
		static float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		static uint32_t indices[] = {
			0, 1, 2
		};

		m_VB = std::unique_ptr<VertexBuffer>(VertexBuffer::Create());
		m_VB->SetData(vertices, sizeof(vertices));

		m_IB = std::unique_ptr<IndexBuffer>(IndexBuffer::Create());
		m_IB->SetData(indices, sizeof(indices));

		m_Shader.reset(Shader::Create("Resources/Shaders/shader.glsl"));
	}

	void EditorLayer::OnDetach()
	{}

	void EditorLayer::OnUpdate()
	{
		using namespace Luma;
		Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

		m_Shader->Bind();
		m_VB->Bind();
		m_IB->Bind();
		Renderer::DrawIndexed(3);
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