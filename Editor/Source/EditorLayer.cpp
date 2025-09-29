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
		Renderer::Clear(0.2f, 0.3f, 0.8f, 1);
	}

	void EditorLayer::OnEvent(Event& e)
	{}

}