#include "EditorLayer.hpp"

#include <iostream>

namespace Luma {

	EditorLayer::EditorLayer()
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
		Renderer::Clear(0.8f, 0.3f, 0.8f, 1);

		float r = 1.0f;
		float g = 0.0f;
		float b = 1.0f;
		LM_RENDER_III(r, g, b, {
			Luma::RendererAPI::Clear(r, g, b, 1.0f);
		});
	}

	void EditorLayer::OnImGuiRender()
	{
		//ImGui::Begin("Example Window");
		//ImGui::Text("Hello World!");
		//ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{}

}