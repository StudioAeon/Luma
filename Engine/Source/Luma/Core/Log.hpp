#pragma once

#include "Luma/Core/Base.hpp"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

namespace Luma {

	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

template<typename OStream>
OStream& operator<<(OStream& os, const glm::vec3& vec)
{
	return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
}

template<typename OStream>
OStream& operator<<(OStream& os, const glm::vec4& vec)
{
	return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
}

// Core
#define LM_CORE_TRACE(...)   ::Luma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LM_CORE_INFO(...)    ::Luma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LM_CORE_WARN(...)    ::Luma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LM_CORE_ERROR(...)   ::Luma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LM_CORE_FATAL(...)   ::Luma::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client
#define LM_TRACE(...)        ::Luma::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LM_INFO(...)         ::Luma::Log::GetClientLogger()->info(__VA_ARGS__)
#define LM_WARN(...)         ::Luma::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LM_ERROR(...)        ::Luma::Log::GetClientLogger()->error(__VA_ARGS__)
#define LM_FATAL(...)        ::Luma::Log::GetClientLogger()->critical(__VA_ARGS__)