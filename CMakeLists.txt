cmake_minimum_required(VERSION 3.25...4.0 FATAL_ERROR)
project(Luma  LANGUAGES C CXX)

# ==== Modern CMake Policies ====
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.31")
	cmake_policy(SET CMP0171 NEW)
	cmake_policy(SET CMP0163 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
	cmake_policy(SET CMP0163 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
	cmake_policy(SET CMP0118 NEW)
endif()

cmake_policy(SET CMP0069 NEW)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ==== Build Config ====
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

# ==== Interprocedural Optimization (Per-target approach) ====
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)

if(IPO_SUPPORTED)
	message(STATUS "Interprocedural Optimization (LTO): Available for per-target use")
else()
	message(WARNING "Interprocedural Optimization (LTO): Not supported by compiler")
endif()

# ==== CMake Utilities ====
include(BuildSystem/CMake/CMakeUtils/LumaCompiler.cmake)
include(BuildSystem/CMake/CMakeUtils/LumaOptimization.cmake)
include(BuildSystem/CMake/CMakeUtils/LumaFileUtils.cmake)

# ==== Distribution Options ====
option(LUMA_BUILD_TESTS "Build Luma test suite" OFF)


# ==== Add Subsystems ====
add_subdirectory(ThirdParty)
add_subdirectory(Engine)
add_subdirectory(Editor)

if(LUMA_BUILD_TESTS)
	include(CTest)
	add_subdirectory(Tests)
endif()