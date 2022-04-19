#include "assert.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#   include <Windows.h>
#endif

#include <stdio.h>

namespace floral
{
// ----------------------------------------------------------------------------

assert_action_e assertion_report(const_cstr expr, const_cstr file, const u32 line)
{
    return assertion_report_dlg(expr, "", file, line);
}

assert_action_e assertion_report_msg(const_cstr expr, const_cstr msg, const_cstr file, const u32 line)
{
    return assertion_report_dlg(expr, msg, file, line);
}

assert_action_e assertion_report_msgonly(const_cstr msg, const_cstr file, const u32 line)
{
    return assertion_report_dlg("", msg, file, line);
}

assert_action_e assertion_report_dlg(const_cstr title, const_cstr msg, const_cstr file, const u32 line)
{
#if defined(FLORAL_PLATFORM_WINDOWS)
    c8 errorStr[1024];
    sprintf(errorStr, ">> expression: %s\n>> message: %s\n>> location: %s:%d\n", title, msg, file, line);
    OutputDebugStringA(errorStr);
#else
    // TODO
#endif
    return floral::assert_action_e::debug_break;
}

// ----------------------------------------------------------------------------
}

