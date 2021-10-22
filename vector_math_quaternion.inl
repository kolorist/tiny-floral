FLORAL_INLINE quaternionf operator*(const quaternionf& q1, const quaternionf& q2)
{
	quaternionf retQuat;
	retQuat.v = cross(q1.v, q2.v) + q1.s * q2.v + q2.s * q1.v;
	retQuat.s = q1.s * q2.s - dot(q1.v, q2.v);
	return retQuat;
}

FLORAL_INLINE quaternionf construct_quaternion_euler(const f32 i_rx, const f32 i_ry, const f32 i_rz)
{
	// TODO: optimize this!!!
	quaternionf qX, qY, qZ;
	f32 halfThetaX = floral::to_radians(i_rx) / 2.0f;
	f32 halfThetaY = floral::to_radians(i_ry) / 2.0f;
	f32 halfThetaZ = floral::to_radians(i_rz) / 2.0f;
	qX.v = floral::vec3f(-sinf(halfThetaX), 0.0f, 0.0f);
	qX.s = cosf(halfThetaX);
	qY.v = floral::vec3f(0.0f, -sinf(halfThetaY), 0.0f);
	qY.s = cosf(halfThetaY);
	qZ.v = floral::vec3f(0.0f, 0.0f, -sinf(halfThetaZ));
	qZ.s = cosf(halfThetaZ);
	return qX * qY * qZ;
}

FLORAL_INLINE quaternionf construct_quaternion_euler(const vec3f& i_r)
{
	return construct_quaternion_euler(i_r.x, i_r.y, i_r.z);
}

FLORAL_INLINE quaternionf construct_quaternion_axis(const vec3f& i_axis, const f32 i_r)
{
	quaternionf q;
	//vec3f n = normalize(i_axis);
	f32 halfTheta = floral::to_radians(i_r) / 2.0f;
	q.v = -i_axis * sinf(halfTheta);
	q.s = cosf(halfTheta);
	return q;
}

FLORAL_INLINE quaternionf construct_quaternion_axis_rad(const vec3f& i_axis, const f32 i_r)
{
	quaternionf q;
	//vec3f n = normalize(i_axis);
	f32 halfTheta = i_r / 2.0f;
	q.v = -i_axis * sinf(halfTheta);
	q.s = cosf(halfTheta);
	return q;
}

FLORAL_INLINE quaternionf construct_quaternion_v2v(const vec3f& i_v0, const vec3f& i_v1)
{
	static constexpr f32 k_epsilon = 0.001f;
	if (equal(i_v0, i_v1, k_epsilon))
	{
		return construct_quaternion_axis_rad(i_v0, 0.0f);
	}
	else if (equal(i_v0, -i_v1, k_epsilon))
	{
		vec3f v(0.0f);
		if (fabs(i_v0.x) < k_epsilon)
		{
			v.x = 1.0f;
		}
		else if (fabs(i_v0.y) < k_epsilon)
		{
			v.y = 1.0f;
		}
		else
		{
			v.z = 1.0f;
		}
		return construct_quaternion_axis_rad(v, pi);
	}

	vec3f u0 = normalize(i_v0);
	vec3f u1 = normalize(i_v1);
	vec3f v = cross(u0, u1);
	f32 angleRad = acosf(dot(u0, u1));
	return construct_quaternion_axis_rad(v, angleRad);
}

// transfomation
FLORAL_INLINE const mat4x4f to_transform(const quaternionf& i_q)
{
	mat4x4f m;
	m.c[0] = vec4f(1.0f - 2.0f * (i_q.y * i_q.y + i_q.z * i_q.z), 2.0f * (i_q.x * i_q.y - i_q.z * i_q.w), 2.0f * (i_q.x * i_q.z + i_q.y * i_q.w), 0.0f);
	m.c[1] = vec4f(2.0f * (i_q.x * i_q.y + i_q.z * i_q.w), 1.0f - 2.0f * (i_q.x * i_q.x + i_q.z * i_q.z), 2.0f * (i_q.y * i_q.z - i_q.x * i_q.w), 0.0f);
	m.c[2] = vec4f(2.0f * (i_q.x * i_q.z - i_q.y * i_q.w), 2.0f * (i_q.y * i_q.z + i_q.x * i_q.w), 1.0f - 2.0f * (i_q.x * i_q.x + i_q.y * i_q.y), 0.0f);
	m.c[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	// TODO: wtf??? no transpose please!!!
	m = construct_transpose(m);
	return m;
}
