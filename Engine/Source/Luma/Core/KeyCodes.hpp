#pragma once

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>

#include <ostream>
#include <cstdint>

namespace Luma {

	typedef enum class KeyCode : uint16_t
	{
		// Printable keys
		Space = SDLK_SPACE,
		Apostrophe = SDLK_APOSTROPHE,
		Comma = SDLK_COMMA,
		Minus = SDLK_MINUS,
		Period = SDLK_PERIOD,
		Slash = SDLK_SLASH,

		D0 = SDLK_0,
		D1 = SDLK_1,
		D2 = SDLK_2,
		D3 = SDLK_3,
		D4 = SDLK_4,
		D5 = SDLK_5,
		D6 = SDLK_6,
		D7 = SDLK_7,
		D8 = SDLK_8,
		D9 = SDLK_9,

		Semicolon = SDLK_SEMICOLON,
		Equal = SDLK_EQUALS,

		A = SDLK_A,
		B = SDLK_B,
		C = SDLK_C,
		D = SDLK_D,
		E = SDLK_E,
		F = SDLK_F,
		G = SDLK_G,
		H = SDLK_H,
		I = SDLK_I,
		J = SDLK_J,
		K = SDLK_K,
		L = SDLK_L,
		M = SDLK_M,
		N = SDLK_N,
		O = SDLK_O,
		P = SDLK_P,
		Q = SDLK_Q,
		R = SDLK_R,
		S = SDLK_S,
		T = SDLK_T,
		U = SDLK_U,
		V = SDLK_V,
		W = SDLK_W,
		X = SDLK_X,
		Y = SDLK_Y,
		Z = SDLK_Z,

		LeftBracket = SDLK_LEFTBRACKET,
		Backslash = SDLK_BACKSLASH,
		RightBracket = SDLK_RIGHTBRACKET,
		GraveAccent = SDLK_GRAVE,

		// Function keys
		Escape = SDLK_ESCAPE,
		Enter = SDLK_RETURN,
		Tab = SDLK_TAB,
		Backspace = SDLK_BACKSPACE,
		Insert = SDLK_INSERT,
		Delete = SDLK_DELETE,
		Right = SDLK_RIGHT,
		Left = SDLK_LEFT,
		Down = SDLK_DOWN,
		Up = SDLK_UP,
		PageUp = SDLK_PAGEUP,
		PageDown = SDLK_PAGEDOWN,
		Home = SDLK_HOME,
		End = SDLK_END,
		CapsLock = SDLK_CAPSLOCK,
		ScrollLock = SDLK_SCROLLLOCK,
		NumLock = SDLK_NUMLOCKCLEAR,
		PrintScreen = SDLK_PRINTSCREEN,
		Pause = SDLK_PAUSE,
		F1 = SDLK_F1,
		F2 = SDLK_F2,
		F3 = SDLK_F3,
		F4 = SDLK_F4,
		F5 = SDLK_F5,
		F6 = SDLK_F6,
		F7 = SDLK_F7,
		F8 = SDLK_F8,
		F9 = SDLK_F9,
		F10 = SDLK_F10,
		F11 = SDLK_F11,
		F12 = SDLK_F12,
		F13 = SDLK_F13,
		F14 = SDLK_F14,
		F15 = SDLK_F15,
		F16 = SDLK_F16,
		F17 = SDLK_F17,
		F18 = SDLK_F18,
		F19 = SDLK_F19,
		F20 = SDLK_F20,
		F21 = SDLK_F21,
		F22 = SDLK_F22,
		F23 = SDLK_F23,
		F24 = SDLK_F24,

		// Keypad
		KP0 = SDLK_KP_0,
		KP1 = SDLK_KP_1,
		KP2 = SDLK_KP_2,
		KP3 = SDLK_KP_3,
		KP4 = SDLK_KP_4,
		KP5 = SDLK_KP_5,
		KP6 = SDLK_KP_6,
		KP7 = SDLK_KP_7,
		KP8 = SDLK_KP_8,
		KP9 = SDLK_KP_9,
		KPDecimal = SDLK_KP_PERIOD,
		KPDivide = SDLK_KP_DIVIDE,
		KPMultiply = SDLK_KP_MULTIPLY,
		KPSubtract = SDLK_KP_MINUS,
		KPAdd = SDLK_KP_PLUS,
		KPEnter = SDLK_KP_ENTER,
		KPEqual = SDLK_KP_EQUALS,

		// Modifiers
		LeftShift = SDLK_LSHIFT,
		LeftControl = SDLK_LCTRL,
		LeftAlt = SDLK_LALT,
		LeftSuper = SDLK_LGUI,
		RightShift = SDLK_RSHIFT,
		RightControl = SDLK_RCTRL,
		RightAlt = SDLK_RALT,
		RightSuper = SDLK_RGUI,
		Menu = SDLK_MENU
	};

	using Key = KeyCode;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	// Mouse button codes
	enum class MouseButton : uint8_t
	{
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2
	};
}

// Convenience macros
#define LM_KEY_SPACE           ::Luma::Key::Space
#define LM_KEY_APOSTROPHE      ::Luma::Key::Apostrophe    /* ' */
#define LM_KEY_COMMA           ::Luma::Key::Comma         /* , */
#define LM_KEY_MINUS           ::Luma::Key::Minus         /* - */
#define LM_KEY_PERIOD          ::Luma::Key::Period        /* . */
#define LM_KEY_SLASH           ::Luma::Key::Slash         /* / */
#define LM_KEY_0               ::Luma::Key::D0
#define LM_KEY_1               ::Luma::Key::D1
#define LM_KEY_2               ::Luma::Key::D2
#define LM_KEY_3               ::Luma::Key::D3
#define LM_KEY_4               ::Luma::Key::D4
#define LM_KEY_5               ::Luma::Key::D5
#define LM_KEY_6               ::Luma::Key::D6
#define LM_KEY_7               ::Luma::Key::D7
#define LM_KEY_8               ::Luma::Key::D8
#define LM_KEY_9               ::Luma::Key::D9
#define LM_KEY_SEMICOLON       ::Luma::Key::Semicolon     /* ; */
#define LM_KEY_EQUAL           ::Luma::Key::Equal         /* = */
#define LM_KEY_A               ::Luma::Key::A
#define LM_KEY_B               ::Luma::Key::B
#define LM_KEY_C               ::Luma::Key::C
#define LM_KEY_D               ::Luma::Key::D
#define LM_KEY_E               ::Luma::Key::E
#define LM_KEY_F               ::Luma::Key::F
#define LM_KEY_G               ::Luma::Key::G
#define LM_KEY_H               ::Luma::Key::H
#define LM_KEY_I               ::Luma::Key::I
#define LM_KEY_J               ::Luma::Key::J
#define LM_KEY_K               ::Luma::Key::K
#define LM_KEY_L               ::Luma::Key::L
#define LM_KEY_M               ::Luma::Key::M
#define LM_KEY_N               ::Luma::Key::N
#define LM_KEY_O               ::Luma::Key::O
#define LM_KEY_P               ::Luma::Key::P
#define LM_KEY_Q               ::Luma::Key::Q
#define LM_KEY_R               ::Luma::Key::R
#define LM_KEY_S               ::Luma::Key::S
#define LM_KEY_T               ::Luma::Key::T
#define LM_KEY_U               ::Luma::Key::U
#define LM_KEY_V               ::Luma::Key::V
#define LM_KEY_W               ::Luma::Key::W
#define LM_KEY_X               ::Luma::Key::X
#define LM_KEY_Y               ::Luma::Key::Y
#define LM_KEY_Z               ::Luma::Key::Z
#define LM_KEY_LEFT_BRACKET    ::Luma::Key::LeftBracket   /* [ */
#define LM_KEY_BACKSLASH       ::Luma::Key::Backslash     /* \ */
#define LM_KEY_RIGHT_BRACKET   ::Luma::Key::RightBracket  /* ] */
#define LM_KEY_GRAVE_ACCENT    ::Luma::Key::GraveAccent   /* ` */
#define LM_KEY_WORLD_1         ::Luma::Key::World1        /* non-US #1 */
#define LM_KEY_WORLD_2         ::Luma::Key::World2        /* non-US #2 */

/* Function keys */
#define LM_KEY_ESCAPE          ::Luma::Key::Escape
#define LM_KEY_ENTER           ::Luma::Key::Enter
#define LM_KEY_TAB             ::Luma::Key::Tab
#define LM_KEY_BACKSPACE       ::Luma::Key::Backspace
#define LM_KEY_INSERT          ::Luma::Key::Insert
#define LM_KEY_DELETE          ::Luma::Key::Delete
#define LM_KEY_RIGHT           ::Luma::Key::Right
#define LM_KEY_LEFT            ::Luma::Key::Left
#define LM_KEY_DOWN            ::Luma::Key::Down
#define LM_KEY_UP              ::Luma::Key::Up
#define LM_KEY_PAGE_UP         ::Luma::Key::PageUp
#define LM_KEY_PAGE_DOWN       ::Luma::Key::PageDown
#define LM_KEY_HOME            ::Luma::Key::Home
#define LM_KEY_END             ::Luma::Key::End
#define LM_KEY_CAPS_LOCK       ::Luma::Key::CapsLock
#define LM_KEY_SCROLL_LOCK     ::Luma::Key::ScrollLock
#define LM_KEY_NUM_LOCK        ::Luma::Key::NumLock
#define LM_KEY_PRINT_SCREEN    ::Luma::Key::PrintScreen
#define LM_KEY_PAUSE           ::Luma::Key::Pause
#define LM_KEY_F1              ::Luma::Key::F1
#define LM_KEY_F2              ::Luma::Key::F2
#define LM_KEY_F3              ::Luma::Key::F3
#define LM_KEY_F4              ::Luma::Key::F4
#define LM_KEY_F5              ::Luma::Key::F5
#define LM_KEY_F6              ::Luma::Key::F6
#define LM_KEY_F7              ::Luma::Key::F7
#define LM_KEY_F8              ::Luma::Key::F8
#define LM_KEY_F9              ::Luma::Key::F9
#define LM_KEY_F10             ::Luma::Key::F10
#define LM_KEY_F11             ::Luma::Key::F11
#define LM_KEY_F12             ::Luma::Key::F12
#define LM_KEY_F13             ::Luma::Key::F13
#define LM_KEY_F14             ::Luma::Key::F14
#define LM_KEY_F15             ::Luma::Key::F15
#define LM_KEY_F16             ::Luma::Key::F16
#define LM_KEY_F17             ::Luma::Key::F17
#define LM_KEY_F18             ::Luma::Key::F18
#define LM_KEY_F19             ::Luma::Key::F19
#define LM_KEY_F20             ::Luma::Key::F20
#define LM_KEY_F21             ::Luma::Key::F21
#define LM_KEY_F22             ::Luma::Key::F22
#define LM_KEY_F23             ::Luma::Key::F23
#define LM_KEY_F24             ::Luma::Key::F24
#define LM_KEY_F25             ::Luma::Key::F25

/* Keypad */
#define LM_KEY_KP_0            ::Luma::Key::KP0
#define LM_KEY_KP_1            ::Luma::Key::KP1
#define LM_KEY_KP_2            ::Luma::Key::KP2
#define LM_KEY_KP_3            ::Luma::Key::KP3
#define LM_KEY_KP_4            ::Luma::Key::KP4
#define LM_KEY_KP_5            ::Luma::Key::KP5
#define LM_KEY_KP_6            ::Luma::Key::KP6
#define LM_KEY_KP_7            ::Luma::Key::KP7
#define LM_KEY_KP_8            ::Luma::Key::KP8
#define LM_KEY_KP_9            ::Luma::Key::KP9
#define LM_KEY_KP_DECIMAL      ::Luma::Key::KPDecimal
#define LM_KEY_KP_DIVIDE       ::Luma::Key::KPDivide
#define LM_KEY_KP_MULTIPLY     ::Luma::Key::KPMultiply
#define LM_KEY_KP_SUBTRACT     ::Luma::Key::KPSubtract
#define LM_KEY_KP_ADD          ::Luma::Key::KPAdd
#define LM_KEY_KP_ENTER        ::Luma::Key::KPEnter
#define LM_KEY_KP_EQUAL        ::Luma::Key::KPEqual

#define LM_KEY_LEFT_SHIFT      ::Luma::Key::LeftShift
#define LM_KEY_LEFT_CONTROL    ::Luma::Key::LeftControl
#define LM_KEY_LEFT_ALT        ::Luma::Key::LeftAlt
#define LM_KEY_LEFT_SUPER      ::Luma::Key::LeftSuper
#define LM_KEY_RIGHT_SHIFT     ::Luma::Key::RightShift
#define LM_KEY_RIGHT_CONTROL   ::Luma::Key::RightControl
#define LM_KEY_RIGHT_ALT       ::Luma::Key::RightAlt
#define LM_KEY_RIGHT_SUPER     ::Luma::Key::RightSuper
#define LM_KEY_MENU            ::Luma::Key::Menu

// Mouse (TODO: move into separate file probably)
#define LM_MOUSE_BUTTON_LEFT    0
#define LM_MOUSE_BUTTON_RIGHT   1
#define LM_MOUSE_BUTTON_MIDDLE  2