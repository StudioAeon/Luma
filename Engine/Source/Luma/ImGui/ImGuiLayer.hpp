#pragma once

#include "lmpch.hpp"
#include "Luma/Core/Layer.hpp"

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
	private:
		float m_Time = 0.0f;
	};

}