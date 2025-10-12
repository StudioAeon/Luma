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
		auto& window = static_cast<Window&>(Application::Get().GetWindow());

		float x, y;
		SDL_GetMouseState(&x, &y);

		return { x, y };
	}

}