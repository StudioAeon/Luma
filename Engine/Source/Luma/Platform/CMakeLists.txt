# ==== SDL Window Backend (Cross-platform) ====
target_sources(Luma
		PRIVATE
		SDL/SDLWindow.cpp
		SDL/SDLWindow.hpp
)

# ==== Platform-Specific Implementations ====
if(WIN32)
	target_sources(Luma
			PRIVATE
			Windows/WindowsFileSystem.cpp
			Windows/WindowsPlatform.cpp
			Windows/WindowsProcessHelper.cpp
			Windows/WindowsThread.cpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_WINDOWS)

elseif(LINUX)
	target_sources(Luma
			PRIVATE
			Linux/LinuxFileSystem.cpp
			Linux/LinuxPlatform.cpp
			Linux/LinuxProcessHelper.cpp
			Linux/LinuxThread.cpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_LINUX)

else()
	message(FATAL_ERROR "Unsupported platform! Luma Engine currently supports Windows and Linux systems only.")
endif()