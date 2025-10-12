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

if(LUMA_TRACK_MEMORY)
	target_compile_definitions(Luma PUBLIC
			$<$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>:LM_TRACK_MEMORY>
	)
endif()

# ==== Linux-Specific Libraries ====
if(UNIX AND NOT APPLE)
	target_compile_definitions(Luma-Editor PRIVATE
			__EMULATE_UUID
	)

	find_package(PkgConfig QUIET)
	if(PkgConfig_FOUND)
		pkg_check_modules(GTK3 gtk+-3.0)

		if(GTK3_FOUND)
			target_link_libraries(Luma-Editor PRIVATE ${GTK3_LIBRARIES})
			target_include_directories(Luma-Editor PRIVATE ${GTK3_INCLUDE_DIRS})
			target_compile_options(Luma-Editor PRIVATE ${GTK3_CFLAGS_OTHER})
			message(STATUS "  -> GTK+3 linked")
		else()
			message(STATUS "  -> GTK+3 not found (optional)")
		endif()
	endif()

	target_link_libraries(Luma-Editor PRIVATE
			dw
			dl
			unwind
			pthread
	)
endif()

# ==== Dependencies ====
target_link_libraries(Luma-Editor PRIVATE Luma)

# ==== Optimizations ====
luma_enable_lto(Luma-Editor)