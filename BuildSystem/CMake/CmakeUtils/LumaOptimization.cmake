function(luma_enable_lto target)
	message(STATUS "Applying optimizations to: ${target}")

	if(IPO_SUPPORTED)
		set_target_properties(${target} PROPERTIES
				INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
				INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION ON
		)

		if(MSVC)
			target_compile_options(${target} PRIVATE
					$<$<CONFIG:Distribution>:/GL>  # Whole program optimization
			)
			target_link_options(${target} PRIVATE
					$<$<CONFIG:Distribution>:/LTCG>  # Link-time code generation
			)
		elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
			target_compile_options(${target} PRIVATE
					$<$<CONFIG:Distribution>:-flto>
			)
			target_link_options(${target} PRIVATE
					$<$<CONFIG:Distribution>:-flto>
			)
		endif()

		message(STATUS "  -> IPO/LTO enabled for Release and Distribution builds")
	else()
		message(STATUS "  -> IPO/LTO not supported by compiler")
	endif()
endfunction()