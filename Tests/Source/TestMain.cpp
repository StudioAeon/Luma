#include "TestLayer.hpp"

#include "Luma/EntryPoint.hpp"

class TestMain : public Luma::Application
{
public:
	TestMain(const Luma::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		PushLayer(new Luma::TestLayer());
	}
};

Luma::Application* Luma::CreateApplication(int argv, char** argc)
{
	Luma::ApplicationSpecification specification;
	specification.Name = "Test Suite";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.VSync = true;

	return new TestMain(specification);
}