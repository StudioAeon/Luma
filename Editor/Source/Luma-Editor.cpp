#include "EditorLayer.hpp"
#include "Luma/Utilities/FileSystem.hpp"

#include "Luma/EntryPoint.hpp"

class LumaEditorApplication : public Luma::Application
{
public:
	LumaEditorApplication(const Luma::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		// Persistent Storage
		{
			m_PersistentStoragePath = Luma::FileSystem::GetPersistentStoragePath() / "Luma-Editor";

			if (!Luma::FileSystem::Exists(m_PersistentStoragePath))
				Luma::FileSystem::CreateDirectory(m_PersistentStoragePath);
		}

		// Update the LUMA_DIR environment variable every time we launch
		{
			auto workingDirectory = Luma::FileSystem::GetWorkingDirectory();

			if (workingDirectory.stem().string() == "Luma-Editor")
				workingDirectory = workingDirectory.parent_path();

			Luma::FileSystem::SetEnvironmentVariable("LUMA_DIR", workingDirectory.string());
		}

		PushLayer(new Luma::EditorLayer());
	}
private:
	std::filesystem::path m_PersistentStoragePath;
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