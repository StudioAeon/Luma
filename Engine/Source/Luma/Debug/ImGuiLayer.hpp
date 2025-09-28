#pragma once

#include "Luma/Core/Layer.hpp"

#include "Luma/Events/KeyEvent.hpp"
#include "Luma/Events/MouseEvent.hpp"
#include "Luma/Events/ApplicationEvent.hpp"

namespace Luma {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;

		void SetBlockEvents(bool block);

	private:
		bool m_BlockEvents = true;
	};

}