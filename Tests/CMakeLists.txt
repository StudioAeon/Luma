include(FetchContent)

FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.9.1
)

FetchContent_MakeAvailable(Catch2)

# Integration tests (full engine)
set(LUMA_INTEGRATION_TEST_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/Source/TestLayer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/TestLayer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Source/TestMain.cpp
)

add_executable(Luma-Tests ${LUMA_INTEGRATION_TEST_SOURCES})
luma_copy_directory(Luma-Tests Editor/Resources Resources)
target_include_directories(Luma-Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Luma-Tests PRIVATE Luma Catch2::Catch2)

luma_enable_lto(Luma-Tests)