function(apply_msvc_settings target)
	target_compile_definitions(${target} PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
			$<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING>
			$<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>
	)

	target_compile_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/Gy>								# Enable function-level linking
			$<$<CXX_COMPILER_ID:MSVC>:/EHsc>							# Exception handling model
			$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>		# Enable conforming preprocessor
			$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>		# Enable correct __cplusplus macro
			$<$<CXX_COMPILER_ID:MSVC>:/wd4068>						# Disable "Unknown #pragma mark warning"
			$<$<CXX_COMPILER_ID:MSVC>:/MP>								# enables multi-processor compilation
	)

	target_link_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/IGNORE:4099>		# Suppress "no PDB found" warning
	)
endfunction()

function(apply_clang_settings target)
	target_compile_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:Clang>:-Wall>
			$<$<CXX_COMPILER_ID:Clang>:-Wextra>
			$<$<CXX_COMPILER_ID:Clang>:-Wno-unknown-pragmas>	# Ignore unknown #pragma mark
			$<$<CXX_COMPILER_ID:Clang>:-ffunction-sections>
			$<$<CXX_COMPILER_ID:Clang>:-fdata-sections>
	)

	target_link_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:Clang>:-Wl,--gc-sections>		# enables dead code elimination
	)

endfunction()

function(apply_gcc_settings target)
	target_compile_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:GNU>:-Wall>
			$<$<CXX_COMPILER_ID:GNU>:-Wextra>
			$<$<CXX_COMPILER_ID:GNU>:-Wno-unknown-pragmas>		# Ignore unknown #pragma mark
			$<$<CXX_COMPILER_ID:GNU>:-ffunction-sections>
			$<$<CXX_COMPILER_ID:GNU>:-fdata-sections>
	)

	target_link_options(${target} PRIVATE
			$<$<CXX_COMPILER_ID:GNU>:-Wl,--gc-sections>		# enables dead code elimination
	)
endfunction()

function(apply_compiler_settings target)
	apply_msvc_settings(${target})
	apply_clang_settings(${target})
	apply_gcc_settings(${target})
endfunction()