#pragma once

#include "Base.hpp"
#include "Log.hpp"

#ifdef LM_PLATFORM_WINDOWS
	#define LM_DEBUG_BREAK __debugbreak()
#elif defined(LM_COMPILER_CLANG)
	#define LM_DEBUG_BREAK __builtin_debugtrap()
#else
	#define LM_DEBUG_BREAK
#endif

#ifdef LM_DEBUG
	#define LM_ENABLE_ASSERTS
#endif

#define LM_ENABLE_VERIFY
#define LM_ENABLE_ENSURE

#ifdef LM_ENABLE_ASSERTS
	#ifdef LM_COMPILER_CLANG
		#define LM_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Assertion Failed", ##__VA_ARGS__)
		#define LM_ASSERT_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Assertion Failed", ##__VA_ARGS__)
	#else
		#define LM_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
		#define LM_ASSERT_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
	#endif

	#define LM_CORE_ASSERT(condition, ...) do { if(!(condition)) { LM_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } } while(0)
	#define LM_ASSERT(condition, ...) do { if(!(condition)) { LM_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } } while(0)
#else
	#define LM_CORE_ASSERT(condition, ...) ((void) (condition))
	#define LM_ASSERT(condition, ...) ((void) (condition))
#endif

#ifdef LM_ENABLE_VERIFY
	#ifdef LM_COMPILER_CLANG
		#define LM_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Verify Failed", ##__VA_ARGS__)
		#define LM_VERIFY_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Verify Failed", ##__VA_ARGS__)
	#else
		#define LM_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Verify Failed" __VA_OPT__(,) __VA_ARGS__)
		#define LM_VERIFY_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Verify Failed" __VA_OPT__(,) __VA_ARGS__)
	#endif

	#define LM_CORE_VERIFY(condition, ...) do { if(!(condition)) { LM_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } } while(0)
	#define LM_VERIFY(condition, ...) do { if(!(condition)) { LM_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } } while(0)
#else
	#define LM_CORE_VERIFY(condition, ...) ((void) (condition))
	#define LM_VERIFY(condition, ...) ((void) (condition))
#endif

#ifdef LM_ENABLE_ENSURE
	#ifdef LM_COMPILER_CLANG
		#define LM_CORE_ENSURE_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Ensure Failed", ##__VA_ARGS__)
		#define LM_ENSURE_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Ensure Failed", ##__VA_ARGS__)
	#else
		#define LM_CORE_ENSURE_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Core, "Ensure Failed" __VA_OPT__(,) __VA_ARGS__)
		#define LM_ENSURE_MESSAGE_INTERNAL(...)  ::Luma::Log::PrintAssertMessage(::Luma::Log::Type::Client, "Ensure Failed" __VA_OPT__(,) __VA_ARGS__)
	#endif

	#define LM_CORE_ENSURE(condition, ...) [&]{ if(!(condition)) { LM_CORE_ENSURE_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } return (condition); }()
	#define LM_ENSURE(condition, ...) [&]{ if(!(condition)) { LM_ENSURE_MESSAGE_INTERNAL(__VA_ARGS__); LM_DEBUG_BREAK; } return (condition) }()

#else
	#define LM_CORE_ENSURE(condition, ...) (condition)
	#define LM_ENSURE(condition, ...) (condition)
#endif