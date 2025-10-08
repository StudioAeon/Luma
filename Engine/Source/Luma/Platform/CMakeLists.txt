if(WIN32)
	target_sources(Luma
			PRIVATE
			Windows/WindowsInput.cpp
			Windows/WindowsWindow.cpp
			Windows/WindowsWindow.hpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_WINDOWS)

elseif(UNIX)
	target_sources(Luma
			PRIVATE
	)

else()
	message(FATAL_ERROR "Unsupported platform! Luma Engine currently supports Windows and Unix-like systems only.")
endif()