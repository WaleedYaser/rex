#if REX_OS_WASM

#include "rex-core/time.h"

#include <time.h>

namespace rc
{
	i64
	time_milliseconds()
	{
		static f64 prev_milliseconds;

		static bool init = true;
		if (init)
		{
			init = false;
			struct timespec prev_time;
			clock_gettime(CLOCK_MONOTONIC, &prev_time);
			prev_milliseconds = prev_time.tv_sec * 1000 + prev_time.tv_nsec * 0.000001;
		}

		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		f64 now_milliseconds = now.tv_sec * 1000 + now.tv_nsec * 0.000001;

		i64 milliseconds = (i64)(now_milliseconds - prev_milliseconds);
		prev_milliseconds = now_milliseconds;

		return milliseconds;
	}

	void
	sleep(u32 milliseconds)
	{
		struct timespec ts;
		ts.tv_sec = milliseconds / 1000;
		ts.tv_nsec = (milliseconds % 1000) * 1'000'000;
		nanosleep(&ts, 0);
	}
}

#endif
