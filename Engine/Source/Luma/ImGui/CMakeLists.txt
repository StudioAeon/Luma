set(IMGUI_SOURCES
		ImGuiBuild.cpp
		ImGuiLayer.cpp
)

set(IMGUI_HEADERS
		ImGuiLayer.hpp
)

target_sources(Luma
		PRIVATE
		${IMGUI_SOURCES}
		${IMGUI_HEADERS}
)