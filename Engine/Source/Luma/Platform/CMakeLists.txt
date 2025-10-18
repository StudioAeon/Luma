# ==== Platform-Specific Implementations ====
if(WIN32)
	target_sources(Luma
			PRIVATE
			Win32/Win32FileSystem.cpp
			Win32/Win32Platform.cpp
			Win32/Win32ProcessHelper.cpp
			Win32/Win32Thread.cpp

			# temporary - will be replaced with Win32Window
			SDL/SDLInput.cpp
			SDL/SDLWindow.cpp
			SDL/SDLWindow.hpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_WINDOWS)

elseif(UNIX AND NOT APPLE)
	target_sources(Luma
			PRIVATE
			Linux/LinuxFileSystem.cpp
			Linux/LinuxPlatform.cpp
			Linux/LinuxProcessHelper.cpp
			Linux/LinuxThread.cpp

			SDL/SDLInput.cpp
			SDL/SDLWindow.cpp
			SDL/SDLWindow.hpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_LINUX)

else()
	message(FATAL_ERROR "Unsupported platform! Luma Engine currently supports Windows and Linux systems only.")
endif()