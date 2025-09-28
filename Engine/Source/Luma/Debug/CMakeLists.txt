set(DEBUG_SOURCES
		ImGuiLayer.cpp
)

set(DEBUG_HEADERS
		ImGuiLayer.hpp
)

target_sources(Luma
		PRIVATE
		${DEBUG_SOURCES}
		${DEBUG_HEADERS}
)