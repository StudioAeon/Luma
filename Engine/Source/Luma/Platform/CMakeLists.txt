if(WIN32)
	target_sources(Luma
			PRIVATE
			Windows/WindowsFileSystem.cpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_WINDOWS)

elseif(LINUX)
	target_sources(Luma
			PRIVATE
			Linux/LinuxFileSystem.cpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_LINUX)

else()
	message(FATAL_ERROR "Unsupported platform! Luma Engine currently supports Windows and Linux systems only.")
endif()