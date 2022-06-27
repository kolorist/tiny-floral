#pragma once

#include "configs.h"
#include "stdaliases.h"

#include "misc.h"

#include <math.h>
#include <string.h>

namespace floral
{
// ----------------------------------------------------------------------------
// vector
// ----------------------------------------------------------------------------

struct vec2f
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};

		f32 elems[2];
	};

	FLORAL_INLINE vec2f()
		: x(0), y(0)
	{ }

	FLORAL_INLINE vec2f(f32 i_x, f32 i_y)
		: x(i_x), y(i_y)
	{ }
};

struct vec3f
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
		};

		struct
		{
			f32 r;
			f32 g;
			f32 b;
		};

		f32 elems[3];
	};

	FLORAL_INLINE vec3f()
		: x(0), y(0), z(0)
	{ }

	FLORAL_INLINE vec3f(f32 i_value)
		: x(i_value), y(i_value), z(i_value)
	{ }

	FLORAL_INLINE vec3f(f32 i_x, f32 i_y, f32 i_z)
		: x(i_x), y(i_y), z(i_z)
	{ }
};

struct vec4f
{
	static const vec4f zero;

	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};

		struct
		{
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};

		f32 elems[4];
	};

	FLORAL_INLINE vec4f()
		: x(0), y(0), z(0), w(0)
	{ }

	FLORAL_INLINE vec4f(f32 i_x, f32 i_y, f32 i_z, f32 i_w)
		: x(i_x), y(i_y), z(i_z), w(i_w)
	{ }
};

#include "vector_math_vec.inl"

// ----------------------------------------------------------------------------
// matrix
// ----------------------------------------------------------------------------

// column-major
struct mat4x4f
{
	static const mat4x4f zero;
	static const mat4x4f identity;

	union
	{
		vec4f c[4];
		f32 m[4][4]; // m[column][row]
		f32 elems[16];
	};

	FLORAL_INLINE mat4x4f()
		: elems { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	{ }

	FLORAL_INLINE mat4x4f(f32 i_value)
		: mat4x4f()
	{
		m[0][0] = i_value;
		m[1][1] = i_value;
		m[2][2] = i_value;
		m[3][3] = i_value;
	}
};

// 
// ----------------------------------------------------------------------------
// camera
// ----------------------------------------------------------------------------

struct camera_view_t
{
	vec3f position;
	vec3f look_at; // maybe direction or target location
	vec3f up_direction;
};

struct camera_ortho_t
{
	f32 left, right, top, bottom;
	f32 near_plane, far_plane;
};

struct camera_persp_t
{
	f32 near_plane, far_plane;
	f32 fov; // degrees
	f32 aspect_ratio;
};

#include "vector_math_mat.inl"

// ----------------------------------------------------------------------------
// quaternion
// ----------------------------------------------------------------------------

struct quaternionf
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};

		struct
		{
			vec3f v;
			f32 s;
		};
	};

	FLORAL_INLINE quaternionf()
		: x(0), y(0), z(0), w(1)
	{ }
};

#include "vector_math_quaternion.inl"

// ----------------------------------------------------------------------------
}
