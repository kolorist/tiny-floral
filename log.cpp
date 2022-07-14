#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(FLORAL_PLATFORM_WINDOWS)
#	include <Windows.h>
#elif defined(FLORAL_PLATFORM_ANDROID)
#	include <android/log.h>
#else
// TODO
#endif

#include "assert.h"
#include "thread.h"

namespace floral
{
// ----------------------------------------------------------------------------

static mutex_t s_globalMtx = floral::create_mutex();

struct thread_logger_t
{
	thread_logger_t()
		: initialized(false)
	{ }

	bool initialized;
	c8 name[128];
};

thread_local thread_logger_t tl_threadLogger;
thread_local u64 tl_frameIndex = 0;

void initialize_logger(const_cstr i_name)
{
	strcpy(tl_threadLogger.name, i_name);
	tl_threadLogger.initialized = true;
}

void tick_logger()
{
	tl_frameIndex++;
}

void logf(log_level_e i_logLevel, const_cstr i_fmt, ...)
{
	FLORAL_ASSERT(tl_threadLogger.initialized);
	floral::lock_guard_t guard(&s_globalMtx);

	c8 str[4096];
	s32 p = 0;
	s32 rLen = 4000;

#if defined(FLORAL_PLATFORM_ANDROID)
	android_LogPriority prio = ANDROID_LOG_DEBUG;
#endif

	u32 fidx = tl_frameIndex % 1000;

	switch (i_logLevel)
	{
		case log_level_e::verbose:
			p = snprintf(&str[0], rLen, "[%s] [%3d] [verbose] ", tl_threadLogger.name, fidx);
#if defined(FLORAL_PLATFORM_ANDROID)
			prio = ANDROID_LOG_VERBOSE;
#endif
			break;
		case log_level_e::debug:
			p = snprintf(&str[0], rLen, "[%s] [%3d] [debug] ", tl_threadLogger.name, fidx);
#if defined(FLORAL_PLATFORM_ANDROID)
			prio = ANDROID_LOG_DEBUG;
#endif
			break;
		case log_level_e::info:
			p = snprintf(&str[0], rLen, "[%s] [%3d] [info] ", tl_threadLogger.name, fidx);
#if defined(FLORAL_PLATFORM_ANDROID)
			prio = ANDROID_LOG_INFO;
#endif
			break;
		case log_level_e::warning:
			p = snprintf(&str[0], rLen, "[%s] [%3d] [warning] ", tl_threadLogger.name, fidx);
#if defined(FLORAL_PLATFORM_ANDROID)
			prio = ANDROID_LOG_WARN;
#endif
			break;
		case log_level_e::error:
			p = snprintf(&str[0], rLen, "[%s] [%3d] [error] ", tl_threadLogger.name, fidx);
#if defined(FLORAL_PLATFORM_ANDROID)
			prio = ANDROID_LOG_ERROR;
#endif
			break;
		default:
			break;
	}

	rLen -= p;

	va_list args;
	va_start(args, i_fmt);
	rLen -= vsnprintf(&str[p], rLen, i_fmt, args);
	va_end(args);

	if (rLen == 0)
	{
		str[4000] = '.';
		str[4001] = '.';
		str[4002] = '.';
		str[4003] = 0;
	}

	puts(str);
	strcat(str, "\n");

#if defined(FLORAL_PLATFORM_WINDOWS) || defined(FLORAL_PLATFORM_XBOX)
	OutputDebugStringA(str);
#elif defined(FLORAL_PLATFORM_ANDROID)
	__android_log_print(prio, "floral", "%s", str);
#endif
}

// ----------------------------------------------------------------------------
}
