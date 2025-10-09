#pragma once

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>

#include <ostream>

namespace Luma {

	enum class KeyCode : uint16_t
	{
		// Printable keys
		Space = SDL_SCANCODE_SPACE,
		Apostrophe = SDL_SCANCODE_APOSTROPHE,
		Comma = SDL_SCANCODE_COMMA,
		Minus = SDL_SCANCODE_MINUS,
		Period = SDL_SCANCODE_PERIOD,
		Slash = SDL_SCANCODE_SLASH,

		D0 = SDL_SCANCODE_0,
		D1 = SDL_SCANCODE_1,
		D2 = SDL_SCANCODE_2,
		D3 = SDL_SCANCODE_3,
		D4 = SDL_SCANCODE_4,
		D5 = SDL_SCANCODE_5,
		D6 = SDL_SCANCODE_6,
		D7 = SDL_SCANCODE_7,
		D8 = SDL_SCANCODE_8,
		D9 = SDL_SCANCODE_9,

		Semicolon = SDL_SCANCODE_SEMICOLON,
		Equal = SDL_SCANCODE_EQUALS,

		A = SDL_SCANCODE_A,
		B = SDL_SCANCODE_B,
		C = SDL_SCANCODE_C,
		D = SDL_SCANCODE_D,
		E = SDL_SCANCODE_E,
		F = SDL_SCANCODE_F,
		G = SDL_SCANCODE_G,
		H = SDL_SCANCODE_H,
		I = SDL_SCANCODE_I,
		J = SDL_SCANCODE_J,
		K = SDL_SCANCODE_K,
		L = SDL_SCANCODE_L,
		M = SDL_SCANCODE_M,
		N = SDL_SCANCODE_N,
		O = SDL_SCANCODE_O,
		P = SDL_SCANCODE_P,
		Q = SDL_SCANCODE_Q,
		R = SDL_SCANCODE_R,
		S = SDL_SCANCODE_S,
		T = SDL_SCANCODE_T,
		U = SDL_SCANCODE_U,
		V = SDL_SCANCODE_V,
		W = SDL_SCANCODE_W,
		X = SDL_SCANCODE_X,
		Y = SDL_SCANCODE_Y,
		Z = SDL_SCANCODE_Z,

		LeftBracket = SDL_SCANCODE_LEFTBRACKET,
		Backslash = SDL_SCANCODE_BACKSLASH,
		RightBracket = SDL_SCANCODE_RIGHTBRACKET,
		GraveAccent = SDL_SCANCODE_GRAVE,

		// Function keys
		Escape = SDL_SCANCODE_ESCAPE,
		Enter = SDL_SCANCODE_RETURN,
		Tab = SDL_SCANCODE_TAB,
		Backspace = SDL_SCANCODE_BACKSPACE,
		Insert = SDL_SCANCODE_INSERT,
		Delete = SDL_SCANCODE_DELETE,
		Right = SDL_SCANCODE_RIGHT,
		Left = SDL_SCANCODE_LEFT,
		Down = SDL_SCANCODE_DOWN,
		Up = SDL_SCANCODE_UP,
		PageUp = SDL_SCANCODE_PAGEUP,
		PageDown = SDL_SCANCODE_PAGEDOWN,
		Home = SDL_SCANCODE_HOME,
		End = SDL_SCANCODE_END,
		CapsLock = SDL_SCANCODE_CAPSLOCK,
		ScrollLock = SDL_SCANCODE_SCROLLLOCK,
		NumLock = SDL_SCANCODE_NUMLOCKCLEAR,
		PrintScreen = SDL_SCANCODE_PRINTSCREEN,
		Pause = SDL_SCANCODE_PAUSE,
		F1 = SDL_SCANCODE_F1,
		F2 = SDL_SCANCODE_F2,
		F3 = SDL_SCANCODE_F3,
		F4 = SDL_SCANCODE_F4,
		F5 = SDL_SCANCODE_F5,
		F6 = SDL_SCANCODE_F6,
		F7 = SDL_SCANCODE_F7,
		F8 = SDL_SCANCODE_F8,
		F9 = SDL_SCANCODE_F9,
		F10 = SDL_SCANCODE_F10,
		F11 = SDL_SCANCODE_F11,
		F12 = SDL_SCANCODE_F12,
		F13 = SDL_SCANCODE_F13,
		F14 = SDL_SCANCODE_F14,
		F15 = SDL_SCANCODE_F15,
		F16 = SDL_SCANCODE_F16,
		F17 = SDL_SCANCODE_F17,
		F18 = SDL_SCANCODE_F18,
		F19 = SDL_SCANCODE_F19,
		F20 = SDL_SCANCODE_F20,
		F21 = SDL_SCANCODE_F21,
		F22 = SDL_SCANCODE_F22,
		F23 = SDL_SCANCODE_F23,
		F24 = SDL_SCANCODE_F24,

		// Keypad
		KP0 = SDL_SCANCODE_KP_0,
		KP1 = SDL_SCANCODE_KP_1,
		KP2 = SDL_SCANCODE_KP_2,
		KP3 = SDL_SCANCODE_KP_3,
		KP4 = SDL_SCANCODE_KP_4,
		KP5 = SDL_SCANCODE_KP_5,
		KP6 = SDL_SCANCODE_KP_6,
		KP7 = SDL_SCANCODE_KP_7,
		KP8 = SDL_SCANCODE_KP_8,
		KP9 = SDL_SCANCODE_KP_9,
		KPDecimal = SDL_SCANCODE_KP_PERIOD,
		KPDivide = SDL_SCANCODE_KP_DIVIDE,
		KPMultiply = SDL_SCANCODE_KP_MULTIPLY,
		KPSubtract = SDL_SCANCODE_KP_MINUS,
		KPAdd = SDL_SCANCODE_KP_PLUS,
		KPEnter = SDL_SCANCODE_KP_ENTER,
		KPEqual = SDL_SCANCODE_KP_EQUALS,

		// Modifiers
		LeftShift = SDL_SCANCODE_LSHIFT,
		LeftControl = SDL_SCANCODE_LCTRL,
		LeftAlt = SDL_SCANCODE_LALT,
		LeftSuper = SDL_SCANCODE_LGUI,
		RightShift = SDL_SCANCODE_RSHIFT,
		RightControl = SDL_SCANCODE_RCTRL,
		RightAlt = SDL_SCANCODE_RALT,
		RightSuper = SDL_SCANCODE_RGUI,
		Menu = SDL_SCANCODE_MENU
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