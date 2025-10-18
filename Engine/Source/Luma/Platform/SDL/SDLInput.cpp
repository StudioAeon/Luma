#include "lmpch.hpp"
#include "Luma/Core/Input.hpp"
#include "Luma/Core/Window.hpp"

#include "Luma/Core/Application.hpp"
#include "Luma/ImGui/ImGuiEx.hpp"

#include <SDL3/SDL.h>
#include <imgui_internal.h>
#include <magic_enum.hpp>
using namespace magic_enum::bitwise_operators;

namespace Luma {

	void Input::Update()
	{
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Pressed;
	}

	bool Input::IsKeyHeld(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Held;
	}

	bool Input::IsKeyDown(KeyCode keycode)
	{
		const bool* keyboardState = SDL_GetKeyboardState(nullptr);
		SDL_Scancode scancode = static_cast<SDL_Scancode>(keycode);

		bool enableImGui = Application::Get().GetSpecification().EnableImGui;
		if (!enableImGui)
		{
			return keyboardState[scancode];
		}

		SDL_Window* win = Application::Get().GetWindow()->GetNativeWindow();
		ImGuiContext* context = ImGui::GetCurrentContext();
		bool pressed = false;

		for (ImGuiViewport* viewport : context->Viewports)
		{
			if (!viewport->PlatformUserData)
				continue;

			SDL_Window* windowHandle = *(SDL_Window**)viewport->PlatformUserData;
			if (!windowHandle)
				continue;

			// SDL3 has global keyboard state, check if this window has focus
			if (SDL_GetKeyboardFocus() == windowHandle && keyboardState[scancode])
			{
				pressed = true;
				break;
			}
		}

		return pressed;
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Released;
	}

	bool Input::IsKeyToggledOn(KeyCode key)
	{
		if (key == KeyCode::CapsLock)
		{
			return (s_Mods & KeyMods::CapsLock) != KeyMods::None;
		}

		if (key == KeyCode::NumLock)
		{
			return (s_Mods & KeyMods::NumLock) != KeyMods::None;
		}

		if (key == KeyCode::ScrollLock)
		{
			return (s_Mods & KeyMods::ScrollLock) != KeyMods::None;
		}

		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Pressed;
	}

	bool Input::IsMouseButtonHeld(MouseButton button)
	{
		return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Held;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		bool enableImGui = Application::Get().GetSpecification().EnableImGui;
		if (!enableImGui)
		{
			Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
			return mouseState & SDL_BUTTON_MASK(static_cast<int>(button));
		}

		ImGuiContext* context = ImGui::GetCurrentContext();
		bool pressed = false;
		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

		for (ImGuiViewport* viewport : context->Viewports)
		{
			if (!viewport->PlatformUserData)
				continue;

			SDL_Window* windowHandle = *(SDL_Window**)viewport->PlatformUserData;
			if (!windowHandle)
				continue;

			// Check if this window has mouse focus
			if (SDL_GetMouseFocus() == windowHandle && (mouseState & SDL_BUTTON_MASK(static_cast<int>(button))))
			{
				pressed = true;
				break;
			}
		}
		return pressed;
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Released;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return (float)y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		float x, y;
		SDL_GetMouseState(&x, &y);
		return { x, y };
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		SDL_Window* window = Application::Get().GetWindow()->GetNativeWindow();

		switch (mode)
		{
			case CursorMode::Normal:
				SDL_SetWindowRelativeMouseMode(window, false);
				SDL_ShowCursor();
				break;
			case CursorMode::Hidden:
				SDL_SetWindowRelativeMouseMode(window, false);
				SDL_HideCursor();
				break;
			case CursorMode::Locked:
				SDL_SetWindowRelativeMouseMode(window, true);
				break;
		}

		if (Application::Get().GetSpecification().EnableImGui)
			ImGuiEx::SetInputEnabled(mode == CursorMode::Normal);
	}

	CursorMode Input::GetCursorMode()
	{
		SDL_Window* window = Application::Get().GetWindow()->GetNativeWindow();

		if (SDL_GetWindowRelativeMouseMode(window))
			return CursorMode::Locked;

		return SDL_CursorVisible() ? CursorMode::Normal : CursorMode::Hidden;
	}

	void Input::TransitionPressedKeys()
	{
		for (const auto& [key, keyData] : s_KeyData)
		{
			if (keyData.State == KeyState::Pressed)
				UpdateKeyState(key, KeyState::Held);
		}
	}

	void Input::TransitionPressedButtons()
	{
		for (const auto& [button, buttonData] : s_MouseData)
		{
			if (buttonData.State == KeyState::Pressed)
				UpdateButtonState(button, KeyState::Held);
		}
	}

	void Input::UpdateKeyState(KeyCode key, KeyState newState)
	{
		auto& keyData = s_KeyData[key];
		keyData.Key = key;
		keyData.OldState = keyData.State;
		keyData.State = newState;
	}

	void Input::UpdateKeyMods(std::optional<KeyMods> mods)
	{
		if (mods.has_value())
			s_Mods = mods.value();
		else
			s_Mods = KeyMods::None;
	}

	void Input::UpdateButtonState(MouseButton button, KeyState newState)
	{
		auto& mouseData = s_MouseData[button];
		mouseData.Button = button;
		mouseData.OldState = mouseData.State;
		mouseData.State = newState;
	}

	void Input::ClearReleasedKeys()
	{
		for (const auto& [key, keyData] : s_KeyData)
		{
			if (keyData.State == KeyState::Released)
				UpdateKeyState(key, KeyState::None);
		}

		for (const auto& [button, buttonData] : s_MouseData)
		{
			if (buttonData.State == KeyState::Released)
				UpdateButtonState(button, KeyState::None);
		}
	}
}