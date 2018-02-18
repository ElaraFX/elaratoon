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

#ifndef EI_VECTOR4_H
#define EI_VECTOR4_H

#include <ei_util.h>
#include <ei_vector.h>

struct eiVector4 {
	union {
		struct {
			eiScalar	x;
			eiScalar	y;
			eiScalar	z;
			eiScalar	w;
		};
		struct {
			eiScalar	r;
			eiScalar	g;
			eiScalar	b;
			eiScalar	a;
		};
		struct {
			eiVector	xyz;
			eiScalar	__w;
		};
	};

	inline eiScalar & operator[] (unsigned int i)
	{
		return *(&x + i);
	}
	
	inline eiScalar operator[] (unsigned int i) const
	{
		return *(&x + i);
	}

	inline eiVector4 & operator = (eiScalar rhs)
	{
		x = rhs;
		y = rhs;
		z = rhs;
		w = rhs;
		return *this;
	}

	inline eiVector4 operator + (const eiVector4 & rhs) const
	{
		eiVector4 temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		temp.w = w + rhs.w;
		return temp;
	}

	inline eiVector4 & operator += (const eiVector4 & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline eiVector4 operator - (const eiVector4 & rhs) const
	{
		eiVector4 temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		temp.w = w - rhs.w;
		return temp;
	}

	inline eiVector4 & operator -= (const eiVector4 & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline eiVector4 operator * (const eiVector4 & rhs) const
	{
		eiVector4 temp;
		temp.x = x * rhs.x;
		temp.y = y * rhs.y;
		temp.z = z * rhs.z;
		temp.w = w * rhs.w;
		return temp;
	}

	inline eiVector4 & operator *= (const eiVector4 & rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	inline eiVector4 operator / (const eiVector4 & rhs) const
	{
		eiVector4 temp;
		temp.x = x / rhs.x;
		temp.y = y / rhs.y;
		temp.z = z / rhs.z;
		temp.w = w / rhs.w;
		return temp;
	}

	inline eiVector4 & operator /= (const eiVector4 & rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	inline eiVector4 operator + (eiScalar rhs) const
	{
		eiVector4 temp;
		temp.x = x + rhs;
		temp.y = y + rhs;
		temp.z = z + rhs;
		temp.w = w + rhs;
		return temp;
	}

	inline eiVector4 & operator += (eiScalar rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	inline eiVector4 operator - (eiScalar rhs) const
	{
		eiVector4 temp;
		temp.x = x - rhs;
		temp.y = y - rhs;
		temp.z = z - rhs;
		temp.w = w - rhs;
		return temp;
	}

	inline eiVector4 & operator -= (eiScalar rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	inline eiVector4 operator * (eiScalar rhs) const
	{
		eiVector4 temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		temp.z = z * rhs;
		temp.w = w * rhs;
		return temp;
	}

	inline eiVector4 & operator *= (eiScalar rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	inline eiVector4 operator / (eiScalar rhs) const
	{
		eiVector4 temp;
		const eiScalar inv = rcpf(rhs);
		temp.x = x * inv;
		temp.y = y * inv;
		temp.z = z * inv;
		temp.w = w * inv;
		return temp;
	}

	inline eiVector4 & operator /= (eiScalar rhs)
	{
		const eiScalar inv = rcpf(rhs);
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
		return *this;
	}

	inline bool operator == (const eiVector4 & rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	inline bool operator != (const eiVector4 & rhs) const
	{
		return !(*this == rhs);
	}

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		(*this) = 0.0f;
	}

	inline void AddWithWeight(eiVector4 const & src, float weight)
	{
		(*this) += src * weight;
	}
};

inline eiVector4 operator - (const eiVector4 & rhs)
{
	eiVector4 temp;
	temp.x = -rhs.x;
	temp.y = -rhs.y;
	temp.z = -rhs.z;
	temp.w = -rhs.w;
	return temp;
}

inline eiVector4 operator + (eiScalar lhs, const eiVector4 & rhs)
{
	eiVector4 temp;
	temp.x = lhs + rhs.x;
	temp.y = lhs + rhs.y;
	temp.z = lhs + rhs.z;
	temp.w = lhs + rhs.w;
	return temp;
}

inline eiVector4 operator - (eiScalar lhs, const eiVector4 & rhs)
{
	eiVector4 temp;
	temp.x = lhs - rhs.x;
	temp.y = lhs - rhs.y;
	temp.z = lhs - rhs.z;
	temp.w = lhs - rhs.w;
	return temp;
}

inline eiVector4 operator * (eiScalar lhs, const eiVector4 & rhs)
{
	eiVector4 temp;
	temp.x = lhs * rhs.x;
	temp.y = lhs * rhs.y;
	temp.z = lhs * rhs.z;
	temp.w = lhs * rhs.w;
	return temp;
}

inline eiVector4 operator / (eiScalar lhs, const eiVector4 & rhs)
{
	eiVector4 temp;
	temp.x = lhs / rhs.x;
	temp.y = lhs / rhs.y;
	temp.z = lhs / rhs.z;
	temp.w = lhs / rhs.w;
	return temp;
}

inline eiVector4 ei_vector4(eiScalar rhs)
{
	eiVector4 temp;
	temp.x = rhs;
	temp.y = rhs;
	temp.z = rhs;
	temp.w = rhs;
	return temp;
}

inline eiVector4 ei_vector4(eiScalar x, eiScalar y, eiScalar z, eiScalar w)
{
	eiVector4 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.w = w;
	return temp;
}

inline eiBool almost_zero(const eiVector4 & a, eiScalar eps)
{
	return (almost_zero(a.x, eps) && almost_zero(a.y, eps) && almost_zero(a.z, eps) && almost_zero(a.w, eps));
}

inline eiBool almost_equal(const eiVector4 & a, const eiVector4 & b, eiScalar eps)
{
	return (almost_equal(a.x, b.x, eps) && almost_equal(a.y, b.y, eps) && almost_equal(a.z, b.z, eps) && almost_equal(a.w, b.w, eps));
}

inline eiScalar dot(const eiVector4 & a, const eiVector4 & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

inline eiScalar len(const eiVector4 & a)
{
	const eiScalar l = dot(a, a);
	return sqrtf(MAX(0.0f, l));
}

inline eiScalar lensq(const eiVector4 & a)
{
	return dot(a, a);
}

inline eiScalar inv_len(const eiVector4 & a)
{
	return invsqrtf(dot(a, a));
}

inline eiScalar dist(const eiVector4 & a, const eiVector4 & b)
{
	return len(b - a);
}

inline eiScalar distsq(const eiVector4 & a, const eiVector4 & b)
{
	const eiVector4 c = b - a;
	return dot(c, c);
}

inline eiVector4 normalize(const eiVector4 & a)
{
	return a * invsqrtf(dot(a, a));
}

#endif
