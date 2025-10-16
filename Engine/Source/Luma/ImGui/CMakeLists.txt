set(IMGUI_SOURCES
		ImGuiBuild.cpp
		ImGuiLayer.cpp
		ImGuizmo.cpp
		VulkanImGui.cpp
)

set(IMGUI_HEADERS
		ImGui.hpp
		ImGuiLayer.hpp
		ImGuizmo.h
)

target_sources(Luma
		PRIVATE
		${IMGUI_SOURCES}
		${IMGUI_HEADERS}
)