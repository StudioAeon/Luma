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