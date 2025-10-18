set(IMGUI_SOURCES
		ImGuiBuild.cpp
		ImGuiEx.cpp
		ImGuiLayer.cpp
		ImGuizmo.cpp
)

set(IMGUI_HEADERS
		ImGui.hpp
		ImGuiEx.hpp
		ImGuiLayer.hpp
		ImGuizmo.h
)

target_sources(Luma
		PRIVATE
		${IMGUI_SOURCES}
		${IMGUI_HEADERS}
)