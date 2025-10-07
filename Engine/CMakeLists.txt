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

add_subdirectory(${LUMA_SRC_DIR}/Luma/Core)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Editor)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Events)
add_subdirectory(${LUMA_SRC_DIR}/Luma/ImGui)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Math)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Platform)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Renderer)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Scene)

target_precompile_headers(Luma PRIVATE ${LUMA_SRC_DIR}/lmpch.hpp)
target_compile_features(Luma PUBLIC cxx_std_23)
target_include_directories(Luma PUBLIC ${LUMA_SRC_DIR})

target_compile_definitions(Luma PUBLIC
		GLM_FORCE_DEPTH_ZERO_TO_ONE
)

add_definitions(-DSDL_MAIN_HANDLED)
target_link_libraries(Luma
		PUBLIC

		# ==== Core Dependencies ====
		spdlog
		SDL3::SDL3-static
		glad
		imgui
		glm
		stb
		assimp
)

luma_enable_lto(Luma)
luma_set_distribution_config(Luma)
apply_compiler_settings(Luma)