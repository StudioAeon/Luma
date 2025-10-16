#include "lmpch.hpp"
#include "Luma/Core/Input.hpp"

#include "Luma/Core/Application.hpp"

#include <SDL3/SDL.h>

namespace Luma {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		const bool* state = SDL_GetKeyboardState(nullptr);

		SDL_Scancode scancode = static_cast<SDL_Scancode>(keycode);

		return state[scancode];
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

		Uint32 buttonFlag = SDL_BUTTON_MASK(button);

		return (mouseState & buttonFlag) != 0;
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

	// TODO: A better way to do this is to handle it internally, and simply move the cursor the opposite side
	//		of the screen when it reaches the edge
	void Input::SetCursorMode(CursorMode mode)
	{
		auto* window = Application::Get().GetWindow()->GetNativeWindow();

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
				SDL_HideCursor();
				break;
		}
	}

	CursorMode Input::GetCursorMode()
	{
		auto* window = Application::Get().GetWindow()->GetNativeWindow();

		if (SDL_GetWindowRelativeMouseMode(window))
			return CursorMode::Locked;

		if (SDL_CursorVisible())
			return CursorMode::Normal;

		return CursorMode::Hidden;
	}

}