#include "lmpch.hpp"
#include "WindowsInput.hpp"
#include "WindowsWindow.hpp"

#include "Luma/Core/Application.hpp"

#include <SDL3/SDL.h>

namespace Luma {

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);

		SDL_Scancode scancode = static_cast<SDL_Scancode>(keycode);

		return state[scancode];
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
		Uint32 buttonFlag = SDL_BUTTON_MASK(button);

		return (mouseState & buttonFlag) != 0;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		float xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return xpos;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		float xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return ypos;
	}

}