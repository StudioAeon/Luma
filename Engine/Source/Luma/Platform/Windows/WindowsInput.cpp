#include "lmpch.hpp"
#include "Luma/Core/Input.hpp"
#include "WindowsWindow.hpp"

#include "Luma/Core/Application.hpp"

#include <SDL3/SDL.h>

namespace Luma {

	bool Input::IsKeyPressed(int keycode)
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
		float xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return xpos;
	}

	float Input::GetMouseY()
	{
		float xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return ypos;
	}

}