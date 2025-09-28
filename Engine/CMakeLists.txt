set(LUMA_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# ==== Root-level infrastructure files ====
set(LUMA_ROOT_SOURCES
		${LUMA_SRC_DIR}/lmpch.cpp
)

set(LUMA_ROOT_HEADERS
		${LUMA_SRC_DIR}/Luma.hpp
		${LUMA_SRC_DIR}/Luma/EntryPoint.hpp
)

add_library(Luma STATIC)

target_sources(Luma PRIVATE ${LUMA_ROOT_SOURCES} ${LUMA_ROOT_HEADERS})

add_subdirectory(${LUMA_SRC_DIR}/Luma/Core)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Events)
add_subdirectory(${LUMA_SRC_DIR}/Luma/Platform)

target_precompile_headers(Luma PRIVATE ${LUMA_SRC_DIR}/lmpch.hpp)
target_compile_features(Luma PUBLIC cxx_std_23)
target_include_directories(Luma PUBLIC ${LUMA_SRC_DIR})

add_definitions(-DSDL_MAIN_HANDLED)
target_link_libraries(Luma
		PUBLIC

		# ==== Core Dependencies ====
		spdlog
		SDL3::SDL3-static
)

luma_enable_lto(Luma)