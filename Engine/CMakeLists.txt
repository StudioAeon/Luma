set(LUMA_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# ==== Root-level infrastructure files ====
set(LUMA_ROOT_SOURCES
		${LUMA_SRC_DIR}/lmpch.cpp
)

set(LUMA_ROOT_HEADERS
		${LUMA_SRC_DIR}/Luma.hpp
		${LUMA_SRC_DIR}/Luma/EntryPoint.hpp
)

# ==== Third-party implementation files ====
set(LUMA_THIRDPARTY_IMPL_SOURCES
		${LUMA_SRC_DIR}/Luma/ThirdParty/stb_impl.cpp
)

add_library(Luma STATIC)

target_sources(Luma PRIVATE ${LUMA_ROOT_SOURCES} ${LUMA_ROOT_HEADERS} ${LUMA_THIRDPARTY_IMPL_SOURCES})

add_subdirectory(${LUMA_SRC_DIR}/Luma/Asset)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Core)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Debug)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Editor)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Events)
add_subdirectory(${LUMA_SRC_DIR}/Luma/ImGui)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Math)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Platform)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Reflection)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Renderer)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Scene)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Serialization)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Utilities)

# ==== PCH Configuration ====
target_precompile_headers(Luma PRIVATE ${LUMA_SRC_DIR}/lmpch.hpp)

set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/imgui/misc/cpp/imgui_stdlib.cpp
		PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

# ==== Target Configuration ====
target_compile_features(Luma PUBLIC cxx_std_23)
target_include_directories(Luma PUBLIC ${LUMA_SRC_DIR})

target_compile_definitions(Luma PUBLIC
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		SDL_MAIN_HANDLED
)

# ==== Platform-Specific Definitions ====
if(WIN32)
	target_compile_definitions(Luma PUBLIC LM_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
	target_compile_definitions(Luma PUBLIC LM_PLATFORM_LINUX)
endif()

# ==== Compiler Settings ====
luma_compiler_settings(Luma)

# ==== Configuration-Specific Definitions ====
target_compile_definitions(Luma PUBLIC
		$<$<CONFIG:Debug>:LM_DEBUG>
		$<$<CONFIG:Debug>:_DEBUG>

		$<$<CONFIG:Release>:LM_RELEASE>
		$<$<CONFIG:Release>:NDEBUG>

		$<$<CONFIG:Distribution>:LM_DIST>
		$<$<CONFIG:Distribution>:NDEBUG>
)

if(LUMA_TRACK_MEMORY)
	target_compile_definitions(Luma PUBLIC
			$<$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>:LM_TRACK_MEMORY>
	)
endif()

# ==== Linux-Specific Libraries ====
if(UNIX AND NOT APPLE)
	target_compile_definitions(Luma PRIVATE
			__EMULATE_UUID
	)

	target_link_libraries(Luma PRIVATE
			dw
			dl
			unwind
			pthread
	)
endif()

# ==== Dependencies ====
target_link_libraries(Luma
		PUBLIC
		glm
		EnTT::EnTT
		spdlog
		SDL3::SDL3-static
		Vulkan::Vulkan
		Vulkan::shaderc_combined
		OpenGL::GL
		imgui
		yaml-cpp
		choc_lib
		nfd
		glad
		stb
		assimp
		FastNoise
		TracyClient
		backward
		spirv-cross-core
		spirv-cross-glsl
		vma
)

# ==== Optimizations ====
luma_enable_lto(Luma)