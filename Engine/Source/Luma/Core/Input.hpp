#pragma once

#include "KeyCodes.hpp"

#include <magic_enum.hpp>

#include <map>
#include <optional>

namespace Luma {

	enum class KeyMods : uint16_t
	{
		None       = 0x0000,

		// Individual modifier keys
		LShift     = 0x0001,
		RShift     = 0x0002,
		Level5     = 0x0004,
		LCtrl      = 0x0040,
		RCtrl      = 0x0080,
		LAlt       = 0x0100,
		RAlt       = 0x0200,
		LGui       = 0x0400,
		RGui       = 0x0800,

		// Lock keys
		NumLock    = 0x1000,
		CapsLock   = 0x2000,
		Mode       = 0x4000,
		ScrollLock = 0x8000,

		Shift      = LShift | RShift,
		Ctrl       = LCtrl | RCtrl,
		Alt        = LAlt | RAlt,
		Gui        = LGui | RGui,
		Super      = Gui
	};

	struct KeyData
	{
		KeyCode Key;
		KeyState State = KeyState::None;
		KeyState OldState = KeyState::None;
	};

	struct ButtonData
	{
		MouseButton Button;
		KeyState State = KeyState::None;
		KeyState OldState = KeyState::None;
	};

	class Input
	{
	public:
		static void Update();

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyHeld(KeyCode keycode);
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);
		static bool IsKeyToggledOn(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonHeld(MouseButton button);
		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

		static void SetCursorMode(CursorMode mode);
		static CursorMode GetCursorMode();

		// Internal use only...
		static void TransitionPressedKeys();
		static void TransitionPressedButtons();
		static void UpdateKeyState(KeyCode key, KeyState newState);
		static void UpdateKeyMods(std::optional<KeyMods> mods);
		static void UpdateButtonState(MouseButton button, KeyState newState);
		static void ClearReleasedKeys();
	private:
		inline static std::map<KeyCode, KeyData> s_KeyData;
		inline static std::map<MouseButton, ButtonData> s_MouseData;
		inline static KeyMods s_Mods = KeyMods::None;
	};

}

template <>
struct magic_enum::customize::enum_range<Luma::KeyMods>
{
	static constexpr bool is_flags = true;
};