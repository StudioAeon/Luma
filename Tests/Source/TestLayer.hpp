#pragma once

#include "Luma.hpp"

namespace Luma {

	class TestLayer : public Layer
	{
	public:
		TestLayer();
		virtual ~TestLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	};

}