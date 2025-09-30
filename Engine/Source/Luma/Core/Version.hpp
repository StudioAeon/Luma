#pragma once

#include "Base.hpp"

#define LM_VERSION "25.4.0.1"

#ifdef NDEBUG
	#define LM_RELEASE
#else
	#define LM_DEBUG
#endif

// ==== Build Configuration ====
#if defined(LM_DEBUG)
	#define LM_BUILD_CONFIG_NAME "Debug"
#elif defined(LM_RELEASE)
	#define LM_BUILD_CONFIG_NAME "Release"
#elif defined(LM_DIST)
	#define LM_BUILD_CONFIG_NAME "Dist"
#else
	#define LM_BUILD_CONFIG_NAME "Unknown"
#endif

// ==== Build Platform ====
#if defined(LM_PLATFORM_WINDOWS)
	#define LM_BUILD_PLATFORM_NAME "Windows x64"
#elif defined(LM_PLATFORM_LINUX)
	#define LM_BUILD_PLATFORM_NAME "Linux"
#elif defined(LM_PLATFORM_BSD)
	#define LM_BUILD_PLATFORM_NAME "BSD"
#elif defined(LM_PLATFORM_UNIX)
	#define LM_BUILD_PLATFORM_NAME "Unix"
#else
	#define LM_BUILD_PLATFORM_NAME "Unknown"
#endif

#define LM_VERSION_LONG "Luma " LM_VERSION " (" LM_BUILD_PLATFORM_NAME " " LM_BUILD_CONFIG_NAME ")"

// Stable build version (YEAR.SEASON.MAJOR.MINOR) Season is 1(Winter), 2(Spring), 3(Summer), 4(Fall)