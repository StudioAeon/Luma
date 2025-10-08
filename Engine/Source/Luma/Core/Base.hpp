#pragma once

#include <memory>
#include <type_traits>

// ==========================================================================
// Platform Detection
// ==========================================================================

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

// ==========================================================================
// Compiler Detection
// ==========================================================================

#if defined(__clang__)
	#define LM_COMPILER_CLANG
#elif defined(__GNUC__)
	#define LM_COMPILER_GCC
#elif defined(_MSC_VER)
	#define LM_COMPILER_MSVC
#else
#error "Unknown compiler! Luma only supports MSVC, GCC, and Clang."
#endif

// ==========================================================================
// Function Inlining & Static Declaration
// ==========================================================================

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

namespace Luma {

	void InitializeCore();
	void ShutdownCore();

	// ==========================================================================
	// Bit Manipulation
	// ==========================================================================
	template<typename T = uint32_t>
	[[nodiscard]] constexpr T Bit(uint8_t position) noexcept
	{
		static_assert(std::is_unsigned_v<T>, "Bit() requires unsigned integer type");
		static_assert(std::is_integral_v<T>, "Bit() requires integral type");
		return static_cast<T>(1) << position;
	}

	// ==========================================================================
	// Smart Pointer Aliases
	// ==========================================================================
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// ==========================================================================
	// Type Aliases
	// ==========================================================================
	using byte = uint8_t;

}

#define LM_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)