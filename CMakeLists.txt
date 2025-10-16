cmake_minimum_required(VERSION 3.25...4.0 FATAL_ERROR)
project(Luma  LANGUAGES C CXX)

# ==== CMake Module Path ====
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/BuildSystem/CMake")

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

# ==== Build Configuration Types ====
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(IS_MULTI_CONFIG)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")
	message(STATUS "Multi-config generator: Debug, Release, Distribution")
else()
	if(NOT DEFINED CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release")
	endif()

	set(valid_build_types Debug Release Distribution)
	list(FIND valid_build_types "${CMAKE_BUILD_TYPE}" _index)
	if(_index EQUAL -1)
		message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Valid options are: ${valid_build_types}")
	endif()

	message(STATUS "Single-config generator: ${CMAKE_BUILD_TYPE}")
endif()

# ==== Distribution Build Configuration ====
set(CMAKE_MAP_IMPORTED_CONFIG_DISTRIBUTION "Release" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DISTRIBUTION "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DISTRIBUTION "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)

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

# ==== Find system packages ====
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

message(STATUS "Found Vulkan: ${Vulkan_VERSION}")
message(STATUS "Using Vulkan includes: ${Vulkan_INCLUDE_DIRS}")

# ==== Fix for configuration-less Vulkan (common on Linux) ====
if(TARGET Vulkan::Vulkan)
	get_target_property(vulkan_configs Vulkan::Vulkan IMPORTED_CONFIGURATIONS)

	# If Vulkan has no configurations, copy IMPORTED_LOCATION to all configs
	if(NOT vulkan_configs)
		get_target_property(vulkan_location Vulkan::Vulkan IMPORTED_LOCATION)
		if(vulkan_location)
			set_target_properties(Vulkan::Vulkan PROPERTIES
					IMPORTED_LOCATION_DEBUG "${vulkan_location}"
					IMPORTED_LOCATION_RELEASE "${vulkan_location}"
					IMPORTED_LOCATION_DISTRIBUTION "${vulkan_location}"
					IMPORTED_CONFIGURATIONS "Debug;Release;Distribution"
			)
			message(STATUS "Fixed Vulkan::Vulkan for multi-config build")
		endif()
	endif()
endif()

# Same fix for shaderc_combined
if(TARGET Vulkan::shaderc_combined)
	get_target_property(shaderc_configs Vulkan::shaderc_combined IMPORTED_CONFIGURATIONS)

	if(NOT shaderc_configs)
		get_target_property(shaderc_location Vulkan::shaderc_combined IMPORTED_LOCATION)
		if(shaderc_location)
			set_target_properties(Vulkan::shaderc_combined PROPERTIES
					IMPORTED_LOCATION_DEBUG "${shaderc_location}"
					IMPORTED_LOCATION_RELEASE "${shaderc_location}"
					IMPORTED_LOCATION_DISTRIBUTION "${shaderc_location}"
					IMPORTED_CONFIGURATIONS "Debug;Release;Distribution"
			)
			message(STATUS "Fixed Vulkan::shaderc_combined for multi-config build")
		endif()
	endif()
endif()

# ==== CMake Utilities ====
include(CmakeUtils/LumaCompiler)
include(CmakeUtils/LumaOptimization)
include(CmakeUtils/LumaFileUtils)

# ==== Distribution Options ====
option(LUMA_BUILD_TESTS "Build Luma test suite" OFF)
option(LUMA_TRACK_MEMORY "Enable memory tracking" ON)

# ==== Add Subsystems ====
add_subdirectory(ThirdParty)
add_subdirectory(Engine)
add_subdirectory(Editor)

if(LUMA_BUILD_TESTS)
	include(CTest)
	add_subdirectory(Tests)
endif()