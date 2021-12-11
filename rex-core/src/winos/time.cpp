#if REX_OS_WINDOWS

#include "rex-core/time.h"

#include <windows.h>

namespace rc
{
	i64
	time_milliseconds()
	{
		static LARGE_INTEGER frequency = {};
		static LARGE_INTEGER prev_ticks = {};

		static bool init = true;
		if (init)
		{
			init = false;
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&prev_ticks);
		}

		LARGE_INTEGER ticks = {};
		QueryPerformanceCounter(&ticks);

		i64 milliseconds = (ticks.QuadPart - prev_ticks.QuadPart) * 1000 / frequency.QuadPart;
		prev_ticks = ticks;

		return milliseconds;
	}

	void
	sleep(u32 milliseconds)
	{
		static bool init = true;
		if (init)
		{
			init = false;
			timeBeginPeriod(1);
		}

		Sleep(milliseconds);
	}
}

#endif
