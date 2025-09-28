#include "EditorLayer.hpp"

#include "Luma/EntryPoint.hpp"
#include "Luma/Debug/ImGuiLayer.hpp"

class LumaEditorApplication : public Luma::Application
{
public:
	LumaEditorApplication(const Luma::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		PushLayer(new Luma::ImGuiLayer("ImGui"));
	}
};

Luma::Application* Luma::CreateApplication(int argv, char** argc)
{
	ApplicationSpecification specification;
	specification.Name = "Luma-Editor";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;

	return new LumaEditorApplication(specification);
}