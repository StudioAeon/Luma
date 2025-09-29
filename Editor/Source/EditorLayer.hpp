#pragma once

#include "Luma.hpp"

namespace Luma {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		float m_ClearColor[4];
	};

}