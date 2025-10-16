include(FetchContent)

# ==== spdlog ====
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_MSVC_UTF8 ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)

FetchContent_Declare(
		spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG v1.15.3
		SYSTEM
)
FetchContent_MakeAvailable(spdlog)

# ==== SDL3 ====
	set(SDL_AUDIO OFF)
	set(SDL_EXAMPLES OFF)
	set(SDL_SAMPLES OFF)
	set(SDL_TESTS OFF)
	set(SDL_STATIC ON)

	FetchContent_Declare(
			SDL3
			GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
			GIT_TAG release-3.2.22
			SYSTEM
	)
	FetchContent_MakeAvailable(SDL3)

# ==== Glad ====
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glad SYSTEM)

# ==== ImGui ====
set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui)

add_library(imgui STATIC
		${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp

		${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
		${IMGUI_DIR}
		${IMGUI_DIR}/backends
)

target_compile_definitions(imgui PUBLIC
		IMGUI_DEFINE_MATH_OPERATORS
)

target_link_libraries(imgui PUBLIC SDL3::SDL3-static)

# ==== GLM (Header-only library) ====
	set(GLM_BUILD_INSTALL OFF)
	set(GLM_BUILD_TESTS OFF)

	FetchContent_Declare(
			glm
			GIT_REPOSITORY https://github.com/g-truc/glm.git
			GIT_TAG 1.0.1
			SYSTEM
	)
	FetchContent_MakeAvailable(glm)

# ==== STB (Header-only library) ====
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb/include)

# ==== Assimp ====
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_BUILD_DOCS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
		assimp
		GIT_REPOSITORY https://github.com/assimp/assimp.git
		GIT_TAG v6.0.2
		SYSTEM
)
FetchContent_MakeAvailable(assimp)

# ==== EnTT (Header-only ECS) ====
FetchContent_Declare(
		EnTT
		GIT_REPOSITORY https://github.com/skypjack/entt.git
		GIT_TAG v3.15.0
		SYSTEM
)
FetchContent_MakeAvailable(EnTT)

# ==== FastNoise2 ====
set(FASTNOISE2_NOISETOOL OFF)
set(FASTNOISE2_TESTS OFF)

FetchContent_Declare(
		FastNoise2
		GIT_REPOSITORY https://github.com/Auburn/FastNoise2.git
		GIT_TAG        v0.10.0-alpha
		GIT_SHALLOW    TRUE
		SYSTEM
)
FetchContent_MakeAvailable(FastNoise2)

# ==== yaml-cpp ====
set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_CONTRIB OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_BUILD_SHARED_LIBS OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/yaml-cpp SYSTEM)

# ==== choc (Header-only audio/utility library) ====
FetchContent_Declare(
		choc
		GIT_REPOSITORY https://github.com/Tracktion/choc.git
		GIT_TAG b262a1fd6352c0d07f946a7bb7122b644bee7ade
		GIT_SHALLOW TRUE
		SYSTEM
)
FetchContent_MakeAvailable(choc)

add_library(choc_lib INTERFACE)
target_include_directories(choc_lib INTERFACE ${choc_SOURCE_DIR})

if(WIN32)
	target_link_libraries(choc_lib INTERFACE ole32 shlwapi)
elseif(UNIX AND NOT APPLE)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(ALSA REQUIRED alsa)
	target_link_libraries(choc_lib INTERFACE ${ALSA_LIBRARIES})
	target_include_directories(choc_lib INTERFACE ${ALSA_INCLUDE_DIRS})
endif()

# ==== Native File Dialog Extended ====
set(NFD_BUILD_TESTS OFF)
set(NFD_BUILD_SDL2_TESTS OFF)
set(NFD_INSTALL OFF)

FetchContent_Declare(
		nfd
		GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
		GIT_TAG v1.2.1
		SYSTEM
)
FetchContent_MakeAvailable(nfd)

# ==== Tracy ====
if(LUMA_TRACK_MEMORY)
set(TRACY_ENABLE ON)
set(TRACY_ON_DEMAND OFF)
set(TRACY_CALLSTACK ON)
set(TRACY_ONLY_LOCALHOST ON)
set(TRACY_NO_BROADCAST ON)
set(TRACY_NO_VERIFY OFF)
set(TRACY_SYMBOL_OFFLINE_RESOLVE OFF)
else()
	set(TRACY_ENABLE OFF)
endif()

FetchContent_Declare(
		tracy
		GIT_REPOSITORY https://github.com/wolfpld/tracy.git
		GIT_TAG v0.12.2
		GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tracy)

if(TARGET TracyClient)
	luma_enable_lto(TracyClient)
endif()

# ==== Backward-cpp (Stack traces) ====
set(BACKWARD_TESTS OFF)

FetchContent_Declare(
		backward-cpp
		GIT_REPOSITORY https://github.com/bombela/backward-cpp.git
		GIT_TAG v1.6
		SYSTEM
)
FetchContent_MakeAvailable(backward-cpp)

FetchContent_Declare(
		spirv_cross
		GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
		GIT_TAG        vulkan-sdk-1.4.313.0
)

set(SPIRV_CROSS_CLI OFF)
set(SPIRV_CROSS_ENABLE_TESTS OFF)

FetchContent_MakeAvailable(spirv_cross)

# ==== Vulkan Memory Allocator ====
find_package(Vulkan REQUIRED)
add_library(vma STATIC
		${CMAKE_CURRENT_LIST_DIR}/VulkanMemoryAllocator/vk_mem_alloc.cpp
)

target_include_directories(vma PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/VulkanMemoryAllocator
)

target_link_libraries(vma PUBLIC Vulkan::Headers)