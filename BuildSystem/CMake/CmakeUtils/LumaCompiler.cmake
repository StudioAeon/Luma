function(luma_compiler_settings target)
	# ==== MSVC-Specific Settings ====
	if(MSVC)
		target_compile_definitions(${target} PRIVATE
				_CRT_SECURE_NO_WARNINGS
				_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
				NOMINMAX
		)

		target_compile_options(${target} PRIVATE
				/EHsc              # Exception handling
				/Zc:preprocessor   # Conforming preprocessor
				/Zc:__cplusplus    # Correct __cplusplus macro
				/wd4068            # Unknown pragma warning
				/MP                # Multi-processor compilation

				# Debug: No optimization, full symbols
				$<$<CONFIG:Debug>:/Od>          # Disable optimization
				$<$<CONFIG:Debug>:/Zi>          # Full debug symbols
				$<$<CONFIG:Debug>:/RTC1>        # Runtime checks

				# Release: Optimize, some symbols, AVX2
				$<$<CONFIG:Release>:/O2>        # Maximize speed
				$<$<CONFIG:Release>:/Zi>        # Debug symbols
				$<$<CONFIG:Release>:/arch:AVX2> # AVX2 + ISA extensions

				# Distribution: Full optimize, no symbols, AVX2
				$<$<CONFIG:Distribution>:/Ox>   # Maximum optimization
				$<$<CONFIG:Distribution>:/arch:AVX2> # AVX2 + ISA extensions
		)

		target_link_options(${target} PRIVATE
				/IGNORE:4099       # No PDB found warning

				# Debug: Full debug info
				$<$<CONFIG:Debug>:/DEBUG:FULL>

				# Release: Generate debug info
				$<$<CONFIG:Release>:/DEBUG>
				$<$<CONFIG:Release>:/OPT:REF>
				$<$<CONFIG:Release>:/OPT:ICF>

				# Distribution: No debug info, maximum optimization
				$<$<CONFIG:Distribution>:/DEBUG:NONE>
				$<$<CONFIG:Distribution>:/OPT:REF>
				$<$<CONFIG:Distribution>:/OPT:ICF>
		)
	endif()

	# ==== GCC/Clang-Specific Settings ====
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(${target} PRIVATE
				-Wall
				-Wextra
				-Wno-unknown-pragmas

				# Debug: No optimization, full symbols
				$<$<CONFIG:Debug>:-O0>          # No optimization
				$<$<CONFIG:Debug>:-g3>          # Full debug symbols

				# Release: Optimize, some symbols, AVX2 + ISA extensions
				$<$<CONFIG:Release>:-O3>        # Maximum optimization
				$<$<CONFIG:Release>:-g>         # Debug symbols
				$<$<CONFIG:Release>:-mavx2>
				$<$<CONFIG:Release>:-mfma>
				$<$<CONFIG:Release>:-mbmi>
				$<$<CONFIG:Release>:-mbmi2>
				$<$<CONFIG:Release>:-mpopcnt>
				$<$<CONFIG:Release>:-mlzcnt>
				$<$<CONFIG:Release>:-mf16c>

				# Distribution: Full optimize, no symbols, AVX2 + ISA extensions
				$<$<CONFIG:Distribution>:-O3>   # Maximum optimization
				$<$<CONFIG:Distribution>:-mavx2>
				$<$<CONFIG:Distribution>:-mfma>
				$<$<CONFIG:Distribution>:-mbmi>
				$<$<CONFIG:Distribution>:-mbmi2>
				$<$<CONFIG:Distribution>:-mpopcnt>
				$<$<CONFIG:Distribution>:-mlzcnt>
				$<$<CONFIG:Distribution>:-mf16c>
		)

		target_link_options(${target} PRIVATE
				$<$<CONFIG:Release>:-g>
		)
	endif()
endfunction()