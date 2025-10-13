#include "lmpch.hpp"
#include "Luma/Core/Platform.hpp"

namespace Luma {

	struct PlatformData
	{
		uint64_t TimerOffset = 0;
		uint64_t TimerFrequency = 0;
	};

	PlatformData g_PlatformData;

	void Platform::Init()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&g_PlatformData.TimerFrequency);

		uint64_t value;
		QueryPerformanceCounter((LARGE_INTEGER*)&value);
		g_PlatformData.TimerOffset = value;
	}

	double Platform::GetTime()
	{
		uint64_t value;
		QueryPerformanceCounter((LARGE_INTEGER*)&value);

		return (double)(value - g_PlatformData.TimerOffset) / g_PlatformData.TimerFrequency;
	}


}