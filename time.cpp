#include "time.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#	include <Windows.h>
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
#else
// TODO
#endif

// ----------------------------------------------------------------------------
}
