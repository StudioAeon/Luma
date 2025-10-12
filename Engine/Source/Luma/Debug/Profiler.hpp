#pragma once

#define LM_ENABLE_PROFILING !LM_DIST

#if LM_ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

#if LM_ENABLE_PROFILING
#define LM_PROFILE_MARK_FRAME			FrameMark
#define LM_PROFILE_FUNC(...)			ZoneScoped##__VA_OPT__(N(__VA_ARGS__))
#define LM_PROFILE_SCOPE(...)			LM_PROFILE_FUNC(__VA_ARGS__)
#define LM_PROFILE_SCOPE_DYNAMIC(NAME)  ZoneScoped; ZoneName(NAME, strlen(NAME))
#define LM_PROFILE_THREAD(...)          tracy::SetThreadName(__VA_ARGS__)
#else
#define LM_PROFILE_MARK_FRAME
#define LM_PROFILE_FUNC(...)
#define LM_PROFILE_SCOPE(...)
#define LM_PROFILE_SCOPE_DYNAMIC(NAME)
#define LM_PROFILE_THREAD(...)
#endif