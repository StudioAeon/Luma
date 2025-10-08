#pragma once

#include "KeyCodes.hpp"

namespace Luma {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};

}