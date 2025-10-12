#include "TestLayer.hpp"
#include "Luma/Utilities/FileSystem.hpp"
#include "Luma/Utilities/CommandLineParser.hpp"

#include "Luma/EntryPoint.hpp"

class TestMain : public Luma::Application
{
public:
	TestMain(const Luma::ApplicationSpecification& specification)
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

		PushLayer(new Luma::TestLayer());
	}
private:
	std::filesystem::path m_PersistentStoragePath;
};

Luma::Application* Luma::CreateApplication(int argc, char** argv)
{
	Luma::CommandLineParser cli(argc, argv);

	auto cd = cli.GetOpt("C");
	if(!cd.empty()) {
		Luma::FileSystem::SetWorkingDirectory(cd);
	}

	Luma::ApplicationSpecification specification;
	specification.Name = "Test Suite";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.Mode = WindowMode::ExclusiveFullscreen;
	specification.VSync = true;

	return new TestMain(specification);
}