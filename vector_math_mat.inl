FLORAL_INLINE mat4x4f operator*(const mat4x4f& i_a, const mat4x4f& i_b)
{
	mat4x4f tMat;
	tMat.m[0][0] = i_a.m[0][0] * i_b.m[0][0] + i_a.m[1][0] * i_b.m[0][1] + i_a.m[2][0] * i_b.m[0][2] + i_a.m[3][0] * i_b.m[0][3];
	tMat.m[0][1] = i_a.m[0][1] * i_b.m[0][0] + i_a.m[1][1] * i_b.m[0][1] + i_a.m[2][1] * i_b.m[0][2] + i_a.m[3][1] * i_b.m[0][3];
	tMat.m[0][2] = i_a.m[0][2] * i_b.m[0][0] + i_a.m[1][2] * i_b.m[0][1] + i_a.m[2][2] * i_b.m[0][2] + i_a.m[3][2] * i_b.m[0][3];
	tMat.m[0][3] = i_a.m[0][3] * i_b.m[0][0] + i_a.m[1][3] * i_b.m[0][1] + i_a.m[2][3] * i_b.m[0][2] + i_a.m[3][3] * i_b.m[0][3];

	tMat.m[1][0] = i_a.m[0][0] * i_b.m[1][0] + i_a.m[1][0] * i_b.m[1][1] + i_a.m[2][0] * i_b.m[1][2] + i_a.m[3][0] * i_b.m[1][3];
	tMat.m[1][1] = i_a.m[0][1] * i_b.m[1][0] + i_a.m[1][1] * i_b.m[1][1] + i_a.m[2][1] * i_b.m[1][2] + i_a.m[3][1] * i_b.m[1][3];
	tMat.m[1][2] = i_a.m[0][2] * i_b.m[1][0] + i_a.m[1][2] * i_b.m[1][1] + i_a.m[2][2] * i_b.m[1][2] + i_a.m[3][2] * i_b.m[1][3];
	tMat.m[1][3] = i_a.m[0][3] * i_b.m[1][0] + i_a.m[1][3] * i_b.m[1][1] + i_a.m[2][3] * i_b.m[1][2] + i_a.m[3][3] * i_b.m[1][3];

	tMat.m[2][0] = i_a.m[0][0] * i_b.m[2][0] + i_a.m[1][0] * i_b.m[2][1] + i_a.m[2][0] * i_b.m[2][2] + i_a.m[3][0] * i_b.m[2][3];
	tMat.m[2][1] = i_a.m[0][1] * i_b.m[2][0] + i_a.m[1][1] * i_b.m[2][1] + i_a.m[2][1] * i_b.m[2][2] + i_a.m[3][1] * i_b.m[2][3];
	tMat.m[2][2] = i_a.m[0][2] * i_b.m[2][0] + i_a.m[1][2] * i_b.m[2][1] + i_a.m[2][2] * i_b.m[2][2] + i_a.m[3][2] * i_b.m[2][3];
	tMat.m[2][3] = i_a.m[0][3] * i_b.m[2][0] + i_a.m[1][3] * i_b.m[2][1] + i_a.m[2][3] * i_b.m[2][2] + i_a.m[3][3] * i_b.m[2][3];

	tMat.m[3][0] = i_a.m[0][0] * i_b.m[3][0] + i_a.m[1][0] * i_b.m[3][1] + i_a.m[2][0] * i_b.m[3][2] + i_a.m[3][0] * i_b.m[3][3];
	tMat.m[3][1] = i_a.m[0][1] * i_b.m[3][0] + i_a.m[1][1] * i_b.m[3][1] + i_a.m[2][1] * i_b.m[3][2] + i_a.m[3][1] * i_b.m[3][3];
	tMat.m[3][2] = i_a.m[0][2] * i_b.m[3][0] + i_a.m[1][2] * i_b.m[3][1] + i_a.m[2][2] * i_b.m[3][2] + i_a.m[3][2] * i_b.m[3][3];
	tMat.m[3][3] = i_a.m[0][3] * i_b.m[3][0] + i_a.m[1][3] * i_b.m[3][1] + i_a.m[2][3] * i_b.m[3][2] + i_a.m[3][3] * i_b.m[3][3];

	return tMat;
}

FLORAL_INLINE mat4x4f construct_lookat_dir_rh(const vec3f& i_upDir, const vec3f& i_camPos, const vec3f& i_lookAtDir)
{
	mat4x4f m(1.0f);
	vec3f front = -normalize(i_lookAtDir);				// points out of the screen
	vec3f i_right = normalize(cross(i_upDir, front));		// points rightward
	vec3f up = cross(front, i_right);						// points upward

	m.m[0][0] = i_right.x;
	m.m[1][0] = i_right.y;
	m.m[2][0] = i_right.z;

	m.m[0][1] = up.x;
	m.m[1][1] = up.y;
	m.m[2][1] = up.z;

	m.m[0][2] = front.x;
	m.m[1][2] = front.y;
	m.m[2][2] = front.z;

	m.m[3][0] = -dot(i_right, i_camPos);
	m.m[3][1] = -dot(up, i_camPos);
	m.m[3][2] = -dot(front, i_camPos);

	return m;
}

FLORAL_INLINE mat4x4f construct_lookat_dir_lh(const vec3f& i_upDir, const vec3f& i_camPos, const vec3f& i_lookAtDir)
{
	mat4x4f m(1.0f);
	vec3f front = normalize(i_lookAtDir);
	vec3f i_right = normalize(cross(i_upDir, front));
	vec3f up = cross(front, i_right);

	m.m[0][0] = i_right.x;
	m.m[1][0] = i_right.y;
	m.m[2][0] = i_right.z;

	m.m[0][1] = up.x;
	m.m[1][1] = up.y;
	m.m[2][1] = up.z;

	m.m[0][2] = front.x;
	m.m[1][2] = front.y;
	m.m[2][2] = front.z;

	m.m[3][0] = -dot(i_right, i_camPos);
	m.m[3][1] = -dot(up, i_camPos);
	m.m[3][2] = -dot(front, i_camPos);

	return m;
}

FLORAL_INLINE mat4x4f construct_lookat_point_rh(const vec3f& i_upDir, const vec3f& i_camPos, const vec3f& lookAtPoint)
{
	vec3f i_lookAtDir = lookAtPoint - i_camPos;
	return construct_lookat_dir_rh(i_upDir, i_camPos, i_lookAtDir);
}

FLORAL_INLINE mat4x4f construct_lookat_point_lh(const vec3f& i_upDir, const vec3f& i_camPos, const vec3f& lookAtPoint)
{
	vec3f i_lookAtDir = lookAtPoint - i_camPos;
	return construct_lookat_dir_lh(i_upDir, i_camPos, i_lookAtDir);
}

FLORAL_INLINE mat4x4f construct_lookat_dir_rh(const camera_view_t* i_desc)
{
	return construct_lookat_dir_rh(i_desc->up_direction, i_desc->position, i_desc->look_at);
}

FLORAL_INLINE mat4x4f construct_lookat_dir_lh(const camera_view_t* i_desc)
{
	return construct_lookat_dir_lh(i_desc->up_direction, i_desc->position, i_desc->look_at);
}

FLORAL_INLINE mat4x4f construct_lookat_point_rh(const camera_view_t* i_desc)
{
	return construct_lookat_point_rh(i_desc->up_direction, i_desc->position, i_desc->look_at);
}

FLORAL_INLINE mat4x4f construct_lookat_point_lh(const camera_view_t* i_desc)
{
	return construct_lookat_point_lh(i_desc->up_direction, i_desc->position, i_desc->look_at);
}

FLORAL_INLINE mat4x4f construct_orthographic_rh(const f32 i_left, const f32 i_right, const f32 i_top, const f32 i_bottom, const f32 i_near, const f32 i_far)
{
	mat4x4f m(1.0f);
	m.m[0][0] = 2.0f / (i_right - i_left);
	m.m[1][1] = 2.0f / (i_top - i_bottom);
	m.m[2][2] = -2.0f / (i_far - i_near);
	m.m[3][0] = -(i_right + i_left) / (i_right - i_left);
	m.m[3][1] = -(i_top + i_bottom) / (i_top - i_bottom);
	m.m[3][2] = -(i_far + i_near) / (i_far - i_near);
	return m;
}

FLORAL_INLINE mat4x4f construct_orthographic_lh(const f32 i_left, const f32 i_right, const f32 i_top, const f32 i_bottom, const f32 i_near, const f32 i_far)
{
	mat4x4f m(1.0f);
	m.m[0][0] = 2.0f / (i_right - i_left);
	m.m[1][1] = 2.0f / (i_top - i_bottom);
	m.m[2][2] = 2.0f / (i_far - i_near);
	m.m[3][0] = -(i_right + i_left) / (i_right - i_left);
	m.m[3][1] = -(i_top + i_bottom) / (i_top - i_bottom);
	m.m[3][2] = -(i_far + i_near) / (i_far - i_near);
	return m;
}

FLORAL_INLINE mat4x4f construct_orthographic_rh(const camera_ortho_t& i_desc)
{
	return construct_orthographic_rh(i_desc.left, i_desc.right, i_desc.top, i_desc.bottom, i_desc.near_plane, i_desc.far_plane);
}

FLORAL_INLINE mat4x4f construct_orthographic_lh(const camera_ortho_t& i_desc)
{
	return construct_orthographic_lh(i_desc.left, i_desc.right, i_desc.top, i_desc.bottom, i_desc.near_plane, i_desc.far_plane);
}

FLORAL_INLINE mat4x4f construct_perspective_rh(const f32 i_near, const f32 i_far, const f32 i_fovy, const f32 i_aspectRatio)
{
	const f32 tanHalfFovY = tanf(to_radians(i_fovy / 2.0f));

	mat4x4f m;
	m.m[0][0] = 1.0f / (i_aspectRatio * tanHalfFovY);
	m.m[1][1] = 1.0f / tanHalfFovY;
	m.m[2][2] = -(i_far + i_near) / (i_far - i_near);
	m.m[2][3] = -1.0f;
	m.m[3][2] = -2.0f * i_far * i_near / (i_far - i_near);

	return m;
}

FLORAL_INLINE mat4x4f construct_infinity_perspective_rh(const f32 i_near, const f32 i_fovy, const f32 i_aspectRatio)
{
	const f32 tanHalfFovY = tanf(to_radians(i_fovy / 2.0f));

	mat4x4f m;
	m.m[0][0] = 1.0f / (i_aspectRatio * tanHalfFovY);
	m.m[1][1] = 1.0f / tanHalfFovY;
	m.m[2][2] = -1;
	m.m[2][3] = -1.0f;
	m.m[3][2] = -2.0f * i_near;

	return m;
}

FLORAL_INLINE mat4x4f construct_perspective_lh(const f32 i_near, const f32 i_far, const f32 i_fovy, const f32 i_aspectRatio)
{
	const f32 tanHalfFovY = tanf(to_radians(i_fovy / 2.0f));

	mat4x4f m;
	m.m[0][0] = 1.0f / (i_aspectRatio * tanHalfFovY);
	m.m[1][1] = 1.0f / tanHalfFovY;
	m.m[2][2] = (i_far + i_near) / (i_far - i_near);
	m.m[2][3] = 1.0f;
	m.m[3][2] = -2.0f * i_far * i_near / (i_far - i_near);

	return m;
}

FLORAL_INLINE mat4x4f construct_infinity_perspective_lh(const f32 i_near, const f32 i_fovy, const f32 i_aspectRatio)
{
	const f32 tanHalfFovY = tanf(to_radians(i_fovy / 2.0f));

	mat4x4f m;
	m.m[0][0] = 1.0f / (i_aspectRatio * tanHalfFovY);
	m.m[1][1] = 1.0f / tanHalfFovY;
	m.m[2][2] = 1.0f;
	m.m[2][3] = 1.0f;
	m.m[3][2] = -2.0f * i_near;

	return m;
}

FLORAL_INLINE mat4x4f construct_perspective_rh(const camera_persp_t* i_desc)
{
	return construct_perspective_rh(i_desc->near_plane, i_desc->far_plane, i_desc->fov, i_desc->aspect_ratio);
}

FLORAL_INLINE mat4x4f construct_perspective_lh(const camera_persp_t* i_desc)
{
	return construct_perspective_lh(i_desc->near_plane, i_desc->far_plane, i_desc->fov, i_desc->aspect_ratio);
}
