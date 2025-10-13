#pragma once

#include <string>

namespace Luma {

	class Platform
	{
	public:
		static void Init();

		static uint64_t GetCurrentDateTimeU64();
		static std::string GetCurrentDateTimeString();

		static double GetTime();

	};

}