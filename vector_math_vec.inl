FLORAL_INLINE const vec3f operator-(const vec3f i_v)
{
	return vec3f(-i_v.x, -i_v.y, -i_v.z);
}

FLORAL_INLINE const vec3f operator-(const vec3f& i_a, const vec3f& i_b)
{
	return vec3f(i_a.x - i_b.x, i_a.y - i_b.y, i_a.z - i_b.z);
}

FLORAL_INLINE const vec3f operator+(const vec3f& i_a, const vec3f& i_b)
{
	return vec3f(i_a.x + i_b.x, i_a.y + i_b.y, i_a.z + i_b.z);
}

// scalar arithmetic
FLORAL_INLINE const vec3f operator*(const vec3f& i_a, const f32 i_scalar)
{
	return vec3f(i_a.x * i_scalar, i_a.y * i_scalar, i_a.z * i_scalar);
}

FLORAL_INLINE const vec3f operator*(const f32 i_scalar, const vec3f& i_a)
{
	return vec3f(i_a.x * i_scalar, i_a.y * i_scalar, i_a.z * i_scalar);
}

FLORAL_INLINE const f32 length(const vec2f& i_v)
{
	return sqrtf(i_v.x * i_v.x + i_v.y * i_v.y);
}

FLORAL_INLINE const f32 length(const vec3f& i_v)
{
	return sqrtf(i_v.x * i_v.x + i_v.y * i_v.y + i_v.z * i_v.z);
}

FLORAL_INLINE const f32 length(const vec4f& i_v)
{
	return sqrtf(i_v.x * i_v.x + i_v.y * i_v.y + i_v.z * i_v.z + i_v.w * i_v.w);
}

FLORAL_INLINE const vec2f normalize(const vec2f& i_v)
{
	f32 len = length(i_v);
	return vec2f(i_v.x / len, i_v.y / len);
}

FLORAL_INLINE const vec3f normalize(const vec3f& i_v)
{
	f32 len = length(i_v);
	return vec3f(i_v.x / len, i_v.y / len, i_v.z / len);
}

FLORAL_INLINE const vec4f normalize(const vec4f& i_v)
{
	f32 len = length(i_v);
	return vec4f(i_v.x / len, i_v.y / len, i_v.z / len, i_v.w / len);
}

FLORAL_INLINE const f32 dot(const vec3f& i_a, const vec3f& i_b)
{
	return (i_a.x * i_b.x + i_a.y * i_b.y + i_a.z * i_b.z);
}

FLORAL_INLINE const vec3f cross(const vec3f& i_a, const vec3f& i_b)
{
	return vec3f(
		i_a.y * i_b.z - i_a.z * i_b.y,
		i_a.z * i_b.x - i_a.x * i_b.z,
		i_a.x * i_b.y - i_a.y * i_b.x);
}

FLORAL_INLINE const bool equal(const vec3f& i_a, const vec3f& i_b, const f32 i_epsilon)
{
	f32 disp = length(i_a - i_b);
	return (disp < i_epsilon);
}
