#pragma once

#include "Base.hpp"
#include "Log.hpp"

#include "Version.hpp"

#ifdef LM_PLATFORM_WINDOWS
	#define LM_DEBUG_BREAK __debugbreak()
#elif defined(LM_COMPILER_CLANG)
	#define LM_DEBUG_BREAK __builtin_debugtrap()
#else
	#define LM_DEBUG_BREAK
#endif

#ifdef LM_DEBUG
	#define LM_ENABLE_ASSERTS
	#define LM_EXPAND_VARGS(x) x
#endif

#ifdef LM_ENABLE_ASSERTS
	#define LM_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { LM_ERROR("Assertion Failed"); __debugbreak(); } }
	#define LM_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { LM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#define LM_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define LM_GET_ASSERT_MACRO(...) LM_EXPAND_VARGS(LM_ASSERT_RESOLVE(__VA_ARGS__, LM_ASSERT_MESSAGE, LM_ASSERT_NO_MESSAGE))

	#define LM_ASSERT(...) LM_EXPAND_VARGS( LM_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
	#define LM_CORE_ASSERT(...) LM_EXPAND_VARGS( LM_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define LM_ASSERT(...)
	#define LM_CORE_ASSERT(...)
#endif