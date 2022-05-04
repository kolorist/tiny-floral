#include "time.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#	include <Windows.h>
#elif defined(FLORAL_PLATFORM_LINUX)
#	include <time.h>
#endif

namespace floral
{
// ----------------------------------------------------------------------------

#if defined(FLORAL_PLATFORM_WINDOWS)
struct windows_time_t
{
	LARGE_INTEGER perfFreq;
	LARGE_INTEGER startTime;
};

typedef windows_time_t platform_time_t;
#elif defined(FLORAL_PLATFORM_LINUX)
	struct linux_time_t
	{
		timespec startTime;
	};

	typedef linux_time_t platform_time_t;
#else
// TODO
#endif

static platform_time_t initialize_time();
static platform_time_t s_platformTime = initialize_time();

// ----------------------------------------------------------------------------

#if defined(FLORAL_PLATFORM_WINDOWS)
platform_time_t initialize_time()
{
	platform_time_t platformTime;
	QueryPerformanceFrequency(&platformTime.perfFreq);
	QueryPerformanceCounter(&platformTime.startTime);

	return platformTime;
}

const f32 get_time_absolute()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	INT64 ticks = currentTime.QuadPart - s_platformTime.startTime.QuadPart;

	f32 absTime = (f32)ticks / (f32)s_platformTime.perfFreq.QuadPart;
	absTime *= 1000.0f;
	return absTime;
}
#elif defined(FLORAL_PLATFORM_LINUX)
platform_time_t initialize_time()
{
	platform_time_t platformTime;
	clock_gettime(CLOCK_MONOTONIC, &platformTime.startTime);
	return platformTime;
}

const f32 get_time_absolute()
{
	timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	timespec diff;
	if ((currentTime.tv_nsec - s_platformTime.startTime.tv_nsec) < 0)
	{
		diff.tv_sec = currentTime.tv_sec - s_platformTime.startTime.tv_sec - 1;
		diff.tv_nsec = 1000000000 + currentTime.tv_nsec - s_platformTime.startTime.tv_nsec;
	}
	else
	{
		diff.tv_sec = currentTime.tv_sec - s_platformTime.startTime.tv_sec;
		diff.tv_nsec = currentTime.tv_nsec - s_platformTime.startTime.tv_nsec;
	}

	f32 diffMillis = diff.tv_sec * 1000.0f + (f32)diff.tv_nsec / 1000000.0f;
	return diffMillis;
}
#else
// TODO
#endif

// ----------------------------------------------------------------------------
}
