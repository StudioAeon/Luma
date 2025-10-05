set(IMGUI_SOURCES
		ImGuiBuild.cpp
		ImGuiLayer.cpp
		ImGuizmo.cpp
)

set(IMGUI_HEADERS
		ImGuiLayer.hpp
		ImGuizmo.h
)

target_sources(Luma
		PRIVATE
		${IMGUI_SOURCES}
		${IMGUI_HEADERS}
)