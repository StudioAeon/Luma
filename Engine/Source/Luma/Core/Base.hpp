#pragma once

#include <functional>
#include <memory>

namespace Luma
{
	void InitializeCore();
	void ShutdownCore();
};

#if defined(_WIN64) || defined(_WIN32)
	#define LM_PLATFORM_WINDOWS
#elif defined(__linux__)
	#define LM_PLATFORM_LINUX
	#define LM_PLATFORM_UNIX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	#define LM_PLATFORM_BSD
	#define LM_PLATFORM_UNIX
#elif defined(__unix__) || defined(__unix)
	#define LM_PLATFORM_UNIX
#else
#error "Unsupported platform! Luma supports Windows, Linux, and BSD."
#endif

#define BIT(x) (1u << x)

//------------------------------------------------------------------------------
// Compiler Detection
//------------------------------------------------------------------------------

#if defined(__clang__)
	#define LM_COMPILER_CLANG
#elif defined(__GNUC__)
	#define LM_COMPILER_GCC
#elif defined(_MSC_VER)
	#define LM_COMPILER_MSVC
#else
#error "Unknown compiler! Luma only supports MSVC, GCC, and Clang."
#endif

//------------------------------------------------------------------------------
// Function Inlining & Static Declaration
//------------------------------------------------------------------------------

#if defined(LM_COMPILER_MSVC)
	#define LM_FORCE_INLINE    __forceinline
	#define LM_EXPLICIT_STATIC static
#elif defined(LM_COMPILER_GCC) || defined(LM_COMPILER_CLANG)
	#define LM_FORCE_INLINE    __attribute__((always_inline)) inline
	#define LM_EXPLICIT_STATIC
#else
	#define LM_FORCE_INLINE    inline
	#define LM_EXPLICIT_STATIC
#endif