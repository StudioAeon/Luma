if(WIN32)
	target_sources(Luma
			PRIVATE
			Windows/WindowsInput.cpp
			Windows/WindowsInput.hpp
			Windows/WindowsWindow.cpp
			Windows/WindowsWindow.hpp
	)

	target_compile_definitions(Luma PRIVATE LM_PLATFORM_WINDOWS)

else()
	message(FATAL_ERROR "Unsupported platform! Luma Engine currently supports Windows and Unix-like systems only.")
endif()