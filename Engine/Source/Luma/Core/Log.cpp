#include "lmpch.hpp"
#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define LM_HAS_CONSOLE !LM_DIST

namespace Luma {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	std::map<std::string, Log::TagDetails> Log::s_DefaultTagDetails = {
		{ "Assimp",            TagDetails{  true, Level::Error } },
		{ "Core",              TagDetails{  true, Level::Trace } },
		{ "SDL",               TagDetails{  true, Level::Error } },
		{ "Mesh",              TagDetails{  true, Level::Warn  } },
		{ "Renderer",          TagDetails{  true, Level::Info  } },
		{ "Scene",             TagDetails{  true, Level::Info  } },
		{ "Timer",             TagDetails{  false, Level::Trace } },
	};

	void Log::Init()
	{
		// Create "logs" directory if doesn't exist
		std::string logsDirectory = "logs";
		if (!std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> lumaSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/LUMA.log", true),
#if LM_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		std::vector<spdlog::sink_ptr> appSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true),
#if LM_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		lumaSinks[0]->set_pattern("[%T] [%l] %n: %v");
		appSinks[0]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("LUMA", lumaSinks.begin(), lumaSinks.end());
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", appSinks.begin(), appSinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);

		SetDefaultTagSettings();
	}

	void Log::Shutdown()
	{
		s_ClientLogger.reset();
		s_CoreLogger.reset();
		spdlog::drop_all();
	}

	void Log::SetDefaultTagSettings()
	{
		s_EnabledTags = s_DefaultTagDetails;
	}

}