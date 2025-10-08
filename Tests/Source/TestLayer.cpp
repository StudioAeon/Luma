#include "TestLayer.hpp"

namespace Luma {

	TestLayer::TestLayer()
	{
	}

	TestLayer::~TestLayer()
	{
	}

	void TestLayer::OnAttach()
	{
	}

	void TestLayer::OnDetach()
	{
	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		Renderer::Clear();
	}

	void TestLayer::OnImGuiRender()
	{
	}

	void TestLayer::OnEvent(Event& e)
	{
	}

}