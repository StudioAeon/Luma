set(DEBUG_HEADERS
		Profiler.hpp
		RenderDoc.hpp
)

target_sources(Luma
		PRIVATE
		${DEBUG_HEADERS}
)