#include "Luma.hpp"

#include "Luma/EntryPoint.hpp"

class LumaEditorApplication : public Luma::Application
{
public:
	LumaEditorApplication(const Luma::ApplicationSpecification& specification)
		: Application(specification)
	{}
};

Luma::Application* Luma::CreateApplication(int argv, char** argc)
{
	ApplicationSpecification specification;
	specification.Name = "Luma-Editor";
	specification.WindowWidth = 1920;
	specification.WindowHeight = 1080;

	return new LumaEditorApplication(specification);
}