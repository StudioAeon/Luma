# ==== Fetch Catch2 Testing Framework ====
include(FetchContent)

FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.9.1
		SYSTEM
)

FetchContent_MakeAvailable(Catch2)

# ==== Source Directory ====
set(TESTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# ==== Shared Test Configuration Function ====
function(luma_configure_test_target TARGET_NAME)
	# Platform-specific definitions
	if(WIN32)
		target_compile_definitions(${TARGET_NAME} PRIVATE LM_PLATFORM_WINDOWS)
	elseif(UNIX AND NOT APPLE)
		target_compile_definitions(${TARGET_NAME} PRIVATE LM_PLATFORM_LINUX)
	endif()

	# Compiler settings
	luma_compiler_settings(${TARGET_NAME})

	# Configuration-specific definitions
	target_compile_definitions(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Debug>:LM_DEBUG>
			$<$<CONFIG:Debug>:_DEBUG>

			$<$<CONFIG:Release>:LM_RELEASE>
			$<$<CONFIG:Release>:NDEBUG>

			$<$<CONFIG:Distribution>:LM_DIST>
			$<$<CONFIG:Distribution>:NDEBUG>
	)

	# Linux-specific libraries
	if(UNIX AND NOT APPLE)
		target_link_libraries(${TARGET_NAME} PRIVATE dl pthread)
	endif()

	# Optimizations
	luma_enable_lto(${TARGET_NAME})
endfunction()

# ==== Unit Tests (No Engine Initialization) ====
set(LUMA_UNIT_TEST_SOURCES
		${TESTS_SRC_DIR}/Core/UUIDTest.cpp
		${TESTS_SRC_DIR}/Core/TimestepTest.cpp
		${TESTS_SRC_DIR}/Core/BufferTest.cpp

		${TESTS_SRC_DIR}/Math/RayTest.cpp
)

if(LUMA_UNIT_TEST_SOURCES)
	add_executable(Luma-UnitTests ${LUMA_UNIT_TEST_SOURCES})

	target_include_directories(Luma-UnitTests PRIVATE
			${CMAKE_SOURCE_DIR}/Engine/Source
			${TESTS_SRC_DIR}
	)

	target_link_libraries(Luma-UnitTests PRIVATE
			Luma
			Catch2::Catch2WithMain
	)

	luma_configure_test_target(Luma-UnitTests)

	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
	include(Catch)
	catch_discover_tests(Luma-UnitTests)
endif()

# ==== Integration Tests (Full Engine) ====
set(LUMA_INTEGRATION_TEST_SOURCES
		${TESTS_SRC_DIR}/TestMain.cpp
		${TESTS_SRC_DIR}/TestLayer.cpp
)

set(LUMA_INTEGRATION_TEST_HEADERS
		${TESTS_SRC_DIR}/TestLayer.hpp
)

add_executable(Luma-Tests
		${LUMA_INTEGRATION_TEST_SOURCES}
		${LUMA_INTEGRATION_TEST_HEADERS}
)

# ==== Resources ====
luma_copy_directory(Luma-Tests Editor/Resources Resources)

# ==== Target Configuration ====
target_include_directories(Luma-Tests PRIVATE ${TESTS_SRC_DIR})

# ==== Dependencies ====
target_link_libraries(Luma-Tests PRIVATE
		Luma
		Catch2::Catch2
)

luma_configure_test_target(Luma-Tests)

# ==== CTest Integration ====
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

if(TARGET Luma-UnitTests)
	catch_discover_tests(Luma-UnitTests)
endif()