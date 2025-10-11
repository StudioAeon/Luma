#pragma once

#include "Luma/Core/Base.hpp"
#include "Luma/Core/LogCustomFormatters.hpp"

#include <spdlog/spdlog.h>

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#ifdef LM_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

namespace Luma {

	class Log
	{
	public:
		enum class Type : uint8_t
		{
			Core = 0, Client = 1
		};
		enum class Level : uint8_t
		{
			Trace = 0, Info, Warn, Error, Fatal
		};
		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static bool HasTag(const std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::map<std::string, TagDetails>& EnabledTags() { return s_EnabledTags; }
		static void SetDefaultTagSettings();

		template<typename... Args>
		static void PrintMessage(Log::Type type, Log::Level level, std::format_string<Args...> format, Args&&... args);

		template<typename... Args>
		static void PrintMessageTag(Log::Type type, Log::Level level, std::string_view tag, std::format_string<Args...> format, Args&&... args);

		static void PrintMessageTag(Log::Type type, Log::Level level, std::string_view tag, std::string_view message);

		template<typename... Args>
		static void PrintAssertMessage(Log::Type type, std::string_view prefix, std::format_string<Args...> message, Args&&... args);

		static void PrintAssertMessage(Log::Type type, std::string_view prefix);

	public:
		// Enum utils
		static const char* LevelToString(Level level)
		{
			switch (level)
			{
				case Level::Trace: return "Trace";
				case Level::Info:  return "Info";
				case Level::Warn:  return "Warn";
				case Level::Error: return "Error";
				case Level::Fatal: return "Fatal";
			}
			return "";
		}
		static Level LevelFromString(std::string_view string)
		{
			if (string == "Trace") return Level::Trace;
			if (string == "Info")  return Level::Info;
			if (string == "Warn")  return Level::Warn;
			if (string == "Error") return Level::Error;
			if (string == "Fatal") return Level::Fatal;

			return Level::Trace;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		inline static std::map<std::string, TagDetails> s_EnabledTags;
		static std::map<std::string, TagDetails> s_DefaultTagDetails;
	};

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tagged logs (prefer these!)                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core logging
#define LM_CORE_TRACE_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Core, ::Luma::Log::Level::Trace, tag, __VA_ARGS__)
#define LM_CORE_INFO_TAG(tag, ...)  ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Core, ::Luma::Log::Level::Info, tag, __VA_ARGS__)
#define LM_CORE_WARN_TAG(tag, ...)  ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Core, ::Luma::Log::Level::Warn, tag, __VA_ARGS__)
#define LM_CORE_ERROR_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Core, ::Luma::Log::Level::Error, tag, __VA_ARGS__)
#define LM_CORE_FATAL_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Core, ::Luma::Log::Level::Fatal, tag, __VA_ARGS__)

// Client logging
#define LM_TRACE_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Client, ::Luma::Log::Level::Trace, tag, __VA_ARGS__)
#define LM_INFO_TAG(tag, ...)  ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Client, ::Luma::Log::Level::Info, tag, __VA_ARGS__)
#define LM_WARN_TAG(tag, ...)  ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Client, ::Luma::Log::Level::Warn, tag, __VA_ARGS__)
#define LM_ERROR_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Client, ::Luma::Log::Level::Error, tag, __VA_ARGS__)
#define LM_FATAL_TAG(tag, ...) ::Luma::Log::PrintMessageTag(::Luma::Log::Type::Client, ::Luma::Log::Level::Fatal, tag, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core Logging
#define LM_CORE_TRACE(...)  ::Luma::Log::PrintMessage(::Luma::Log::Type::Core, ::Luma::Log::Level::Trace, __VA_ARGS__)
#define LM_CORE_INFO(...)   ::Luma::Log::PrintMessage(::Luma::Log::Type::Core, ::Luma::Log::Level::Info, __VA_ARGS__)
#define LM_CORE_WARN(...)   ::Luma::Log::PrintMessage(::Luma::Log::Type::Core, ::Luma::Log::Level::Warn, __VA_ARGS__)
#define LM_CORE_ERROR(...)  ::Luma::Log::PrintMessage(::Luma::Log::Type::Core, ::Luma::Log::Level::Error, __VA_ARGS__)
#define LM_CORE_FATAL(...)  ::Luma::Log::PrintMessage(::Luma::Log::Type::Core, ::Luma::Log::Level::Fatal, __VA_ARGS__)

// Client Logging
#define LM_TRACE(...)   ::Luma::Log::PrintMessage(::Luma::Log::Type::Client, ::Luma::Log::Level::Trace, __VA_ARGS__)
#define LM_INFO(...)    ::Luma::Log::PrintMessage(::Luma::Log::Type::Client, ::Luma::Log::Level::Info, __VA_ARGS__)
#define LM_WARN(...)    ::Luma::Log::PrintMessage(::Luma::Log::Type::Client, ::Luma::Log::Level::Warn, __VA_ARGS__)
#define LM_ERROR(...)   ::Luma::Log::PrintMessage(::Luma::Log::Type::Client, ::Luma::Log::Level::Error, __VA_ARGS__)
#define LM_FATAL(...)   ::Luma::Log::PrintMessage(::Luma::Log::Type::Client, ::Luma::Log::Level::Fatal, __VA_ARGS__)

namespace Luma {

	template<typename... Args>
	void Log::PrintMessage(Log::Type type, Log::Level level, std::format_string<Args...> format, Args&&... args)
	{
		auto detail = s_EnabledTags[""];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();

			// Pre-format the message with the provided format and arguments before calling the logger.
			// This allows the code to compile on a wider range of compilers (notably, Clang with libstdc++ on Linux)
			std::string formatted = std::format(format, std::forward<Args>(args)...);
			switch (level)
			{
			case Level::Trace:
				logger->trace(formatted);
				break;
			case Level::Info:
				logger->info(formatted);
				break;
			case Level::Warn:
				logger->warn(formatted);
				break;
			case Level::Error:
				logger->error(formatted);
				break;
			case Level::Fatal:
				logger->critical(formatted);
				break;
			}
		}
	}


	template<typename... Args>
	void Log::PrintMessageTag(Log::Type type, Log::Level level, std::string_view tag, const std::format_string<Args...> format, Args&&... args)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string formatted = std::format(format, std::forward<Args>(args)...);
			switch (level)
			{
				case Level::Trace:
					logger->trace("[{0}] {1}", tag, formatted);
					break;
				case Level::Info:
					logger->info("[{0}] {1}", tag, formatted);
					break;
				case Level::Warn:
					logger->warn("[{0}] {1}", tag, formatted);
					break;
				case Level::Error:
					logger->error("[{0}] {1}", tag, formatted);
					break;
				case Level::Fatal:
					logger->critical("[{0}] {1}", tag, formatted);
					break;
			}
		}
	}


	inline void Log::PrintMessageTag(Log::Type type, Log::Level level, std::string_view tag, std::string_view message)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			switch (level)
			{
				case Level::Trace:
					logger->trace("[{0}] {1}", tag, message);
					break;
				case Level::Info:
					logger->info("[{0}] {1}", tag, message);
					break;
				case Level::Warn:
					logger->warn("[{0}] {1}", tag, message);
					break;
				case Level::Error:
					logger->error("[{0}] {1}", tag, message);
					break;
				case Level::Fatal:
					logger->critical("[{0}] {1}", tag, message);
					break;
			}
		}
	}


	template<typename... Args>
	void Log::PrintAssertMessage(Log::Type type, std::string_view prefix, std::format_string<Args...> message, Args&&... args)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		auto formatted = std::format(message, std::forward<Args>(args)...);
		logger->error("{0}: {1}", prefix, formatted);

#ifdef LM_PLATFORM_WINDOWS
		MessageBoxA(nullptr, formatted.data(), "Luma Assert", MB_OK | MB_ICONERROR);
#endif
	}


	inline void Log::PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
#ifdef LM_PLATFORM_WINDOWS
		MessageBoxA(nullptr, "No message :(", "Luma Assert", MB_OK | MB_ICONERROR);
#endif
	}
}