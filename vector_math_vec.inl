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
