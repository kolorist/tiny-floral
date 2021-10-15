#pragma once

#include "configs.h"
#include "stdaliases.h"

namespace floral
{
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
}