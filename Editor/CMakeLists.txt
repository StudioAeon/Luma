set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set(EDITOR_SOURCES
		${EDITOR_SRC_DIR}/EditorLayer.cpp
		${EDITOR_SRC_DIR}/Luma-Editor.cpp
)

set(EDITOR_HEADERS
		${EDITOR_SRC_DIR}/EditorLayer.hpp
		${EDITOR_SRC_DIR}/Luma-Editor.cpp
)

add_executable(Luma-Editor ${EDITOR_SOURCES} ${EDITOR_HEADERS})

set_target_properties(Luma-Editor PROPERTIES
		WIN32_EXECUTABLE $<CONFIG:Release>
)

luma_copy_directory(Luma-Editor Editor/Resources Resources)

target_compile_features(Luma-Editor PRIVATE cxx_std_23)
target_include_directories(Luma-Editor PUBLIC ${EDITOR_SRC_DIR})

target_link_libraries(Luma-Editor PRIVATE Luma)

luma_enable_lto(Luma-Editor)
luma_set_distribution_config(Luma-Editor)
apply_compiler_settings(Luma-Editor)