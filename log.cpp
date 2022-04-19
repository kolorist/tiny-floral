#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(FLORAL_PLATFORM_WINDOWS)
#   include <Windows.h>
#endif

namespace floral
{
// ----------------------------------------------------------------------------

void logf(log_level_e i_logLevel, const_cstr i_fmt, ...)
{
    c8 str[4096];
    s32 p = 0;
    s32 rLen = 4000;

    switch (i_logLevel)
    {
        case log_level_e::verbose:
            p = sprintf_s(&str[0], rLen, "[verbose] ");
            break;
        case log_level_e::debug:
            p = sprintf_s(&str[0], rLen, "[debug] ");
            break;
        case log_level_e::info:
            p = sprintf_s(&str[0], rLen, "[info] ");
            break;
        case log_level_e::warning:
            p = sprintf_s(&str[0], rLen, "[warning] ");
            break;
        case log_level_e::error:
            p = sprintf_s(&str[0], rLen, "[error] ");
            break;
        default:
            break;
    }

    rLen -= p;

    va_list args;
    va_start(args, i_fmt);
    rLen -= vsprintf_s(&str[p], rLen, i_fmt, args);
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
#endif
}

// ----------------------------------------------------------------------------
}
