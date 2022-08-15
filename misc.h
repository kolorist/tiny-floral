#pragma once

#include "configs.h"
#include "stdaliases.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#	include <Windows.h>
#elif defined(FLORAL_PLATFORM_ANDROID)
#	include <android/native_window.h>
#endif

namespace floral
{
// ----------------------------------------------------------------------------

struct native_surface_t
{
#if defined(FLORAL_PLATFORM_WINDOWS)
	HINSTANCE									hInst;
	HWND										hwnd;
#elif defined(FLORAL_PLATFORM_ANDROID)
	ANativeWindow*								wnd;
#endif
};

constexpr f32 pi = 3.1415926525898f;
constexpr f32 half_pi = 1.5707963267948966f;

FLORAL_INLINE const f32 to_radians(const f32 i_degree)
{
    return (i_degree / 180.0f * pi);
}

FLORAL_INLINE const size align_size(const size i_size, const size i_alignment)
{
    if ((i_size & (i_alignment - 1)) == 0)
    {
        return i_size;
    }

    return i_size + (i_alignment - (i_size & (i_alignment - 1)));
}

template <class t_type>
t_type min(t_type lhs, t_type rhs)
{
    return (lhs > rhs ? rhs : lhs);
}

template <class t_type>
t_type max(t_type lhs, t_type rhs)
{
    return (lhs > rhs ? lhs : rhs);
}

template <class t_type>
t_type clamp(t_type d, t_type minValue, t_type maxValue)
{
    return min(maxValue, max(d, minValue));
}

template<typename t_type>
t_type smoothstep(t_type edge0, t_type edge1, t_type x)
{
    x = clamp((x - edge0) / (edge1 - edge0), t_type(0.0f), t_type(1.0f));
    return x * x * (3.0f - 2.0f * x);
}

// ----------------------------------------------------------------------------
}
