#include "TestLayer.hpp"

namespace Luma {

	TestLayer::TestLayer()
	{}

	TestLayer::~TestLayer()
	{}

	void TestLayer::OnAttach()
	{}

	void TestLayer::OnDetach()
	{}

	void TestLayer::OnUpdate()
	{
		Renderer::Clear();
	}

	void TestLayer::OnImGuiRender()
	{}

	void TestLayer::OnEvent(Event& e)
	{}

}