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

    switch (i_logLevel)
    {
        case log_level_e::verbose:
            p = sprintf(&str[0], "[verbose] ");
            break;
        case log_level_e::debug:
            p = sprintf(&str[0], "[debug] ");
            break;
        case log_level_e::info:
            p = sprintf(&str[0], "[info] ");
            break;
        case log_level_e::warning:
            p = sprintf(&str[0], "[warning] ");
            break;
        case log_level_e::error:
            p = sprintf(&str[0], "[error] ");
            break;
        default:
            break;
    }

    va_list args;
    va_start(args, i_fmt);
    vsprintf(&str[p], i_fmt, args);
    va_end(args);

    puts(str);
    strcat(str, "\n");

#if defined(FLORAL_PLATFORM_WINDOWS) || defined(FLORAL_PLATFORM_XBOX)
    OutputDebugStringA(str);
#endif
}

// ----------------------------------------------------------------------------
}
