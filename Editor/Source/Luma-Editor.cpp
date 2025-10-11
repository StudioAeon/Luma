#include "EditorLayer.hpp"

#include "Luma/EntryPoint.hpp"
#include "Luma/ImGui/ImGuiLayer.hpp"

class LumaEditorApplication : public Luma::Application
{
public:
	LumaEditorApplication(const Luma::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		PushLayer(new Luma::EditorLayer());
	}
};

Luma::Application* Luma::CreateApplication(int argv, char** argc)
{
	ApplicationSpecification specification;
	specification.Name = "Luma-Editor";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.Mode = WindowMode::Windowed;
	specification.VSync = true;

	return new LumaEditorApplication(specification);
}