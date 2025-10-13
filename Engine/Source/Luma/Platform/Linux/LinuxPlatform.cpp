#include "lmpch.hpp"
#include "Luma/Core/Platform.hpp"

#include <unistd.h>
#include <sys/time.h>

namespace Luma {

	struct PlatformData
	{
		clockid_t Clock;

		uint64_t TimerOffset = 0;
		uint64_t TimerFrequency = 0;
	};

	PlatformData g_PlatformData;

	void Platform::Init()
	{
		g_PlatformData.Clock = CLOCK_REALTIME;
		g_PlatformData.TimerFrequency = 1000000000;

#if defined(_POSIX_MONOTONIC_CLOCK)
		{
			timespec ts;
			if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
				g_PlatformData.Clock = CLOCK_MONOTONIC;
		}
#endif
		timespec ts;
		clock_gettime(g_PlatformData.Clock, &ts);
		g_PlatformData.TimerOffset = (uint64_t) ts.tv_sec * g_PlatformData.TimerFrequency + (uint64_t) ts.tv_nsec;
	}

	double Platform::GetTime()
	{
		timespec ts;
		clock_gettime(g_PlatformData.Clock, &ts);
		return (double)(((uint64_t) ts.tv_sec * g_PlatformData.TimerFrequency + (uint64_t) ts.tv_nsec)
							- g_PlatformData.TimerOffset) / (double)g_PlatformData.TimerFrequency;
	}

}