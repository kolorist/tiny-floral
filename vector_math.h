#pragma once

#include "configs.h"
#include "stdaliases.h"

namespace floral
{
// ----------------------------------------------------------------------------

struct float2
{
	f32 x;
	f32 y;

	FLORAL_INLINE float2()
		: x(0), y(0)
	{ }

	FLORAL_INLINE float2(f32 i_x, f32 i_y)
		: x(i_x), y(i_y)
	{ }
};

struct float4
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;

	FLORAL_INLINE float4()
		: x(0), y(0), z(0), w(0)
	{ }

	FLORAL_INLINE float4(f32 i_x, f32 i_y, f32 i_z, f32 i_w)
		: x(i_x), y(i_y), z(i_z), w(i_w)
	{ }
};

struct vec4f
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
	union
	{
		f32 elems[4];
	};
};

// ----------------------------------------------------------------------------
}