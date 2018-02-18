/**************************************************************************
 * Copyright (C) 2015 Rendease Co., Ltd.
 * All rights reserved.
 *
 * This program is commercial software: you must not redistribute it 
 * and/or modify it without written permission from Rendease Co., Ltd.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * End User License Agreement for more details.
 *
 * You should have received a copy of the End User License Agreement along 
 * with this program.  If not, see <http://www.rendease.com/licensing/>
 *************************************************************************/

#ifndef EI_VECTOR_H
#define EI_VECTOR_H

#include <ei_util.h>
#include <ei_vector2.h>

#define ei_point	ei_vector
#define ei_normal	ei_vector

struct eiVector {

	eiScalar x, y, z;

	inline eiScalar & operator[] (unsigned int i)
	{
		return *(&x + i);
	}
	
	inline eiScalar operator[] (unsigned int i) const
	{
		return *(&x + i);
	}

	inline eiVector & operator = (eiScalar rhs)
	{
		x = rhs;
		y = rhs;
		z = rhs;
		return *this;
	}

	inline eiVector operator + (const eiVector & rhs) const
	{
		eiVector temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		return temp;
	}

	inline eiVector & operator += (const eiVector & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline eiVector operator - (const eiVector & rhs) const
	{
		eiVector temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		return temp;
	}

	inline eiVector & operator -= (const eiVector & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline eiVector operator * (const eiVector & rhs) const
	{
		eiVector temp;
		temp.x = x * rhs.x;
		temp.y = y * rhs.y;
		temp.z = z * rhs.z;
		return temp;
	}

	inline eiVector & operator *= (const eiVector & rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	inline eiVector operator / (const eiVector & rhs) const
	{
		eiVector temp;
		temp.x = x / rhs.x;
		temp.y = y / rhs.y;
		temp.z = z / rhs.z;
		return temp;
	}

	inline eiVector & operator /= (const eiVector & rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	inline eiVector operator + (eiScalar rhs) const
	{
		eiVector temp;
		temp.x = x + rhs;
		temp.y = y + rhs;
		temp.z = z + rhs;
		return temp;
	}

	inline eiVector & operator += (eiScalar rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	inline eiVector operator - (eiScalar rhs) const
	{
		eiVector temp;
		temp.x = x - rhs;
		temp.y = y - rhs;
		temp.z = z - rhs;
		return temp;
	}

	inline eiVector & operator -= (eiScalar rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	inline eiVector operator * (eiScalar rhs) const
	{
		eiVector temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		temp.z = z * rhs;
		return temp;
	}

	inline eiVector & operator *= (eiScalar rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	inline eiVector operator / (eiScalar rhs) const
	{
		eiVector temp;
		const eiScalar inv = rcpf(rhs);
		temp.x = x * inv;
		temp.y = y * inv;
		temp.z = z * inv;
		return temp;
	}

	inline eiVector & operator /= (eiScalar rhs)
	{
		const eiScalar inv = rcpf(rhs);
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	inline bool operator == (const eiVector & rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline bool operator != (const eiVector & rhs) const
	{
		return !(*this == rhs);
	}

	inline eiScalar average() const
	{
		return (x + y + z) * (1.0f / 3.0f);
	}

	inline eiInt max_axis() const
	{
		eiInt axis = Z_AXIS;

		if (x > y && x > z)
		{
			axis = X_AXIS;
		}
		else if (y > z)
		{
			axis = Y_AXIS;
		}

		return axis;
	}

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		(*this) = 0.0f;
	}

	inline void AddWithWeight(eiVector const & src, float weight)
	{
		(*this) += src * weight;
	}
};

typedef struct eiVector eiPoint;
typedef struct eiVector eiNormal;

inline eiVector operator - (const eiVector & rhs)
{
	eiVector temp;
	temp.x = -rhs.x;
	temp.y = -rhs.y;
	temp.z = -rhs.z;
	return temp;
}

inline eiVector operator + (eiScalar lhs, const eiVector & rhs)
{
	eiVector temp;
	temp.x = lhs + rhs.x;
	temp.y = lhs + rhs.y;
	temp.z = lhs + rhs.z;
	return temp;
}

inline eiVector operator - (eiScalar lhs, const eiVector & rhs)
{
	eiVector temp;
	temp.x = lhs - rhs.x;
	temp.y = lhs - rhs.y;
	temp.z = lhs - rhs.z;
	return temp;
}

inline eiVector operator * (eiScalar lhs, const eiVector & rhs)
{
	eiVector temp;
	temp.x = lhs * rhs.x;
	temp.y = lhs * rhs.y;
	temp.z = lhs * rhs.z;
	return temp;
}

inline eiVector operator / (eiScalar lhs, const eiVector & rhs)
{
	eiVector temp;
	temp.x = lhs / rhs.x;
	temp.y = lhs / rhs.y;
	temp.z = lhs / rhs.z;
	return temp;
}

inline eiVector ei_vector(eiScalar rhs)
{
	eiVector temp;
	temp.x = rhs;
	temp.y = rhs;
	temp.z = rhs;
	return temp;
}

inline eiVector ei_vector(eiScalar x, eiScalar y, eiScalar z)
{
	eiVector temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return temp;
}

inline eiBool almost_zero(const eiVector & a, eiScalar eps)
{
	return (almost_zero(a.x, eps) && almost_zero(a.y, eps) && almost_zero(a.z, eps));
}

inline eiBool almost_equal(const eiVector & a, const eiVector & b, eiScalar eps)
{
	return (almost_equal(a.x, b.x, eps) && almost_equal(a.y, b.y, eps) && almost_equal(a.z, b.z, eps));
}

inline eiVector cross(const eiVector & a, const eiVector & b)
{
	eiVector temp;
	temp.x = a.y * b.z - a.z * b.y;
    temp.y = a.z * b.x - a.x * b.z;
    temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

inline eiScalar dot(const eiVector & a, const eiVector & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline eiScalar len(const eiVector & a)
{
	const eiScalar l = dot(a, a);
	return sqrtf(MAX(0.0f, l));
}

inline eiScalar lensq(const eiVector & a)
{
	return dot(a, a);
}

inline eiScalar inv_len(const eiVector & a)
{
	return invsqrtf(dot(a, a));
}

inline eiScalar dist(const eiVector & a, const eiVector & b)
{
	return len(b - a);
}

inline eiScalar distsq(const eiVector & a, const eiVector & b)
{
	const eiVector c = b - a;
	return dot(c, c);
}

inline eiVector normalize(const eiVector & a)
{
	const __m128 pa = _mm_max_ss(_mm_set_ss(a.x * a.x + a.y * a.y + a.z * a.z), _mm_set_ss(EI_NORM_EPS));
	const __m128 r = _mm_rsqrt_ss(pa);
	/* one more iteration */
	const eiScalar d = _mm_cvtss_f32(_mm_mul_ss(r, _mm_add_ss(_mm_set_ss(1.5f), 
		_mm_mul_ss(_mm_mul_ss(pa, _mm_set_ss(-0.5f)), _mm_mul_ss(r, r)))));
	return a * d;
}

inline eiScalar normalize_len(eiVector & r, const eiVector & a)
{
	const __m128 pa = _mm_sqrt_ss(_mm_max_ss(_mm_set_ss(EI_NORM_EPS), 
		_mm_set_ss(a.x * a.x + a.y * a.y + a.z * a.z)));
	const __m128 pr = _mm_rcp_ss(pa);
	/* one more iteration */
	const eiScalar rd = _mm_cvtss_f32(_mm_sub_ss(_mm_add_ss(pr, pr), _mm_mul_ss(_mm_mul_ss(pr, pr), pa)));
	r = a * rd;
	return _mm_cvtss_f32(pa);
}

inline void ortho_basis(const eiVector & N, eiVector & X, eiVector & Y)
{
	X = (absf(N.x) > 0.01f ? ei_vector(N.z, 0.0f, -N.x) : ei_vector(0.0f, -N.z, N.y));
	X = normalize(X);
	Y = cross(N, X);
}

inline void to_local_frame(
	eiVector & R, const eiVector & D, 
	const eiVector & U, const eiVector & V, const eiVector & N)
{
	R.x = dot(D, U);
	R.y = dot(D, V);
	R.z = dot(D, N);
}

/** Calculate the unsigned triangle area.
 */
inline eiScalar tri_area(const eiVector & a, const eiVector & b, const eiVector & c)
{
	return 0.5f * len(cross(b - a, c - a));
}

inline eiScalar tri_area_normal(const eiVector & a, const eiVector & b, const eiVector & c, eiVector & n)
{
	n = cross(b - a, c - a);
	const eiScalar d = dot(n, n);
	const eiScalar l = sqrtf(MAX(EI_NORM_EPS, d));
	n /= l;
	return 0.5f * l;
}

inline void dir_to_angle(const eiVector & dir, eiScalar & theta, eiScalar & phi)
{
	theta = acosf(dir.z);					/* return value in [0...Pi] */
	phi = atan2f(dir.y, dir.x);				/* return value in [-Pi...Pi] */
	if (phi < 0.0f) {
		phi = phi + 2.0f * (eiScalar)EI_PI;	/* make value in [0...2Pi] */
	}
}

inline void angle_to_dir(eiScalar theta, eiScalar phi, eiVector & dir)
{
	const eiScalar sin_theta = sinf(theta);
	dir.x = sin_theta * cosf(phi);
	dir.y = sin_theta * sinf(phi);
	dir.z = cosf(theta);
}

inline void get_unnormalized_normal(const eiVector & v1, const eiVector & v2, const eiVector & v3, eiVector & normal)
{
	/* get normal vector of a triangle, remember that we are 
	   in right-handed coordinate system by default and clockwise 
	   vertices will form the front face.
	   unnormalized version is faster. */
	normal = cross(v2 - v1, v3 - v1);
}

inline void get_normal(const eiVector & v1, const eiVector & v2, const eiVector & v3, eiVector & normal)
{
	/* get normal vector of a triangle, remember that we are 
	   in right-handed coordinate system by default and clockwise 
	   vertices will form the front face. */
	normal = normalize(cross(v2 - v1, v3 - v1));
}

inline void interp_scalar(eiScalar & p, eiScalar p1, eiScalar p2, eiScalar p3, const eiVector & bary)
{
	p = p1 * bary.x + p2 * bary.y + p3 * bary.z;
}

inline void interp_point(eiVector & p, const eiVector & p1, const eiVector & p2, const eiVector & p3, const eiVector & bary)
{
	p = p1 * bary.x + p2 * bary.y + p3 * bary.z;
}

inline void interp_normal(eiVector & p, const eiVector & p1, const eiVector & p2, const eiVector & p3, const eiVector & bary)
{
	p = normalize(p1 * bary.x + p2 * bary.y + p3 * bary.z);
}

inline eiScalar point_plane_dist(const eiVector & P, const eiVector & N, const eiVector & V)
{
	return dot(N, P - V);
}

inline void point_on_plane(eiVector & result, const eiVector & P, const eiVector & N, const eiVector & V)
{
	result = P + N * (-point_plane_dist(P, N, V));
}

inline eiVector faceforward(const eiVector & N, const eiVector & I, const eiVector & Nref)
{
	return sign(-dot(I, Nref)) * N;
}

template <>
inline eiVector clamp(eiVector x, eiVector lo, eiVector hi)
{
	return ei_vector(
		clamp(x.x, lo.x, hi.x), 
		clamp(x.y, lo.y, hi.y), 
		clamp(x.z, lo.z, hi.z));
}

static const eiVector g_ZeroVector = {0.0f, 0.0f, 0.0f};
static const eiVector g_OneVector = {1.0f, 1.0f, 1.0f};

/* OSL interoperability */
#if defined eiCORE_EXPORTS || defined EI_OSL_INTEROP

inline OSL::Vec3 to_vec3(const eiVector & v)
{
	return OSL::Vec3(v.x, v.y, v.z);
}

inline void to_vec3(OSL::Vec3 & r, const eiVector & v)
{
	r.x = v.x;
	r.y = v.y;
	r.z = v.z;
}

inline eiVector from_vec3(const OSL::Vec3 & c)
{
	return ei_vector(c.x, c.y, c.z);
}

inline void from_vec3(eiVector & v, const OSL::Vec3 & c)
{
	v.x = c.x;
	v.y = c.y;
	v.z = c.z;
}

#endif

EI_API eiScalar ei_perlin(const eiVector & p);
EI_API eiScalar ei_perlin2(const eiVector2 & p);
EI_API eiScalar ei_perlin4(const eiVector & p, eiScalar time);

EI_API void ei_vperlin(eiVector & result, const eiVector & p);
EI_API void ei_vperlin2(eiVector & result, const eiVector2 & p);
EI_API void ei_vperlin4(eiVector & result, const eiVector & p, eiScalar time);

EI_API eiScalar ei_cell_noise(const eiVector & p);
EI_API eiScalar ei_cell_noise2(const eiVector2 & p);
EI_API eiScalar ei_cell_noise4(const eiVector & p, eiScalar time);

EI_API void ei_vcell_noise(eiVector & result, const eiVector & p);
EI_API void ei_vcell_noise2(eiVector & result, const eiVector2 & p);
EI_API void ei_vcell_noise4(eiVector & result, const eiVector & p, eiScalar time);

#endif
