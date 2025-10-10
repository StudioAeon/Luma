#include "lmpch.hpp"
#include "ImGuiLayer.hpp"

#include "Luma/Core/Application.hpp"
#include "Luma/Renderer/Renderer.hpp"

#include <imgui.h>
#include "ImGuizmo.h"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <SDL3/SDL.h>

namespace Luma {

	ImGuiLayer::ImGuiLayer()
	{
	}

	ImGuiLayer::ImGuiLayer(const std::string& name)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());

		// Setup Renderer/Backend/Renderer bindings
		ImGui_ImplSDL3_InitForOpenGL(window, SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Text Colors
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

		// Window & Background
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.0f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.15f, 0.17f, 1.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.11f, 0.13f, 0.95f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.17f, 0.19f, 1.0f);

		// Borders
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.27f, 0.30f, 1.0f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

		// Frame Backgrounds (Inputs, etc.)
		colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.20f, 0.23f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.27f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.30f, 0.34f, 1.0f);

		// Title Bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.15f, 0.17f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.13f, 0.15f, 0.75f);

		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.0f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.37f, 0.40f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.47f, 0.50f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.57f, 0.60f, 1.0f);

		// Interactive Elements
		colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.80f, 1.0f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.80f, 1.0f, 0.8f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.85f, 1.0f, 1.0f);

		// Buttons
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.25f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);

		// Headers (TreeNode, etc.)
		colors[ImGuiCol_Header] = ImVec4(0.22f, 0.24f, 0.27f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.50f, 0.70f, 0.8f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.55f, 0.75f, 1.0f);

		// Separators
		colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.27f, 0.30f, 1.0f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.80f, 1.0f, 0.8f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.80f, 1.0f, 1.0f);

		// Resize Grips
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.27f, 0.30f, 0.7f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.80f, 1.0f, 0.8f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.80f, 1.0f, 1.0f);

		// Plots & Graphs
		colors[ImGuiCol_PlotLines] = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.60f, 0.0f, 1.0f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.80f, 1.0f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.85f, 1.0f, 1.0f);

		// Special Elements
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.40f, 0.80f, 1.0f, 0.9f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.40f, 0.80f, 1.0f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	}

}