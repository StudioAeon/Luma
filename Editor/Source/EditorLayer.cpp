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
	{}

	void EditorLayer::OnEvent(Event& e)
	{}

}