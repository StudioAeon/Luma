set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set(EDITOR_SOURCES
		${EDITOR_SRC_DIR}/EditorLayer.cpp
		${EDITOR_SRC_DIR}/Luma-Editor.cpp
)

set(EDITOR_HEADERS
		${EDITOR_SRC_DIR}/EditorLayer.hpp
)

add_executable(Luma-Editor ${EDITOR_SOURCES} ${EDITOR_HEADERS})

# ==== Win32 Executable (No console in Distribution) ====
if(WIN32)
	set_target_properties(Luma-Editor PROPERTIES
			WIN32_EXECUTABLE $<CONFIG:Distribution>
	)
endif()

# ==== Resources ====
luma_copy_directory(Luma-Editor Editor/Resources Resources)

# ==== Target Configuration ====
target_include_directories(Luma-Editor PUBLIC ${EDITOR_SRC_DIR})

# ==== Platform-Specific Definitions ====
if(WIN32)
	target_compile_definitions(Luma-Editor PRIVATE LM_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
	target_compile_definitions(Luma-Editor PRIVATE LM_PLATFORM_LINUX)
endif()

# ==== Compiler Settings ====
luma_compiler_settings(Luma-Editor)

# ==== Configuration-Specific Definitions ====
target_compile_definitions(Luma-Editor PRIVATE
		$<$<CONFIG:Debug>:LM_DEBUG>
		$<$<CONFIG:Debug>:_DEBUG>

		$<$<CONFIG:Release>:LM_RELEASE>
		$<$<CONFIG:Release>:NDEBUG>

		$<$<CONFIG:Distribution>:LM_DIST>
		$<$<CONFIG:Distribution>:NDEBUG>
)

# ==== Linux-Specific Libraries ====
if(UNIX AND NOT APPLE)
	target_link_libraries(Luma-Editor PRIVATE
			dl
			pthread
	)
endif()

# ==== Dependencies ====
target_link_libraries(Luma-Editor PRIVATE Luma)

# ==== Optimizations ====
luma_enable_lto(Luma-Editor)