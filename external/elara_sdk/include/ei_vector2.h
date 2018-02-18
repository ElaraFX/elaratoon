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

#ifndef EI_VECTOR2_H
#define EI_VECTOR2_H

#include <ei_util.h>

struct eiVector2 {

	eiScalar x, y;

	inline eiScalar & operator[] (unsigned int i)
	{
		return *(&x + i);
	}
	
	inline eiScalar operator[] (unsigned int i) const
	{
		return *(&x + i);
	}

	inline eiVector2 & operator = (eiScalar rhs)
	{
		x = rhs;
		y = rhs;
		return *this;
	}

	inline eiVector2 operator + (const eiVector2 & rhs) const
	{
		eiVector2 temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		return temp;
	}

	inline eiVector2 & operator += (const eiVector2 & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	inline eiVector2 operator - (const eiVector2 & rhs) const
	{
		eiVector2 temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		return temp;
	}

	inline eiVector2 & operator -= (const eiVector2 & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	inline eiVector2 operator * (const eiVector2 & rhs) const
	{
		eiVector2 temp;
		temp.x = x * rhs.x;
		temp.y = y * rhs.y;
		return temp;
	}

	inline eiVector2 & operator *= (const eiVector2 & rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	inline eiVector2 operator / (const eiVector2 & rhs) const
	{
		eiVector2 temp;
		temp.x = x / rhs.x;
		temp.y = y / rhs.y;
		return temp;
	}

	inline eiVector2 & operator /= (const eiVector2 & rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	inline eiVector2 operator + (eiScalar rhs) const
	{
		eiVector2 temp;
		temp.x = x + rhs;
		temp.y = y + rhs;
		return temp;
	}

	inline eiVector2 & operator += (eiScalar rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}

	inline eiVector2 operator - (eiScalar rhs) const
	{
		eiVector2 temp;
		temp.x = x - rhs;
		temp.y = y - rhs;
		return temp;
	}

	inline eiVector2 & operator -= (eiScalar rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}

	inline eiVector2 operator * (eiScalar rhs) const
	{
		eiVector2 temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		return temp;
	}

	inline eiVector2 & operator *= (eiScalar rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	inline eiVector2 operator / (eiScalar rhs) const
	{
		eiVector2 temp;
		const eiScalar inv = rcpf(rhs);
		temp.x = x * inv;
		temp.y = y * inv;
		return temp;
	}

	inline eiVector2 & operator /= (eiScalar rhs)
	{
		const eiScalar inv = rcpf(rhs);
		x *= inv;
		y *= inv;
		return *this;
	}

	inline bool operator == (const eiVector2 & rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	inline bool operator != (const eiVector2 & rhs) const
	{
		return !(*this == rhs);
	}

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		(*this) = 0.0f;
	}

	inline void AddWithWeight(eiVector2 const & src, float weight)
	{
		(*this) += src * weight;
	}
};

inline eiVector2 operator - (const eiVector2 & rhs)
{
	eiVector2 temp;
	temp.x = -rhs.x;
	temp.y = -rhs.y;
	return temp;
}

inline eiVector2 operator + (eiScalar lhs, const eiVector2 & rhs)
{
	eiVector2 temp;
	temp.x = lhs + rhs.x;
	temp.y = lhs + rhs.y;
	return temp;
}

inline eiVector2 operator - (eiScalar lhs, const eiVector2 & rhs)
{
	eiVector2 temp;
	temp.x = lhs - rhs.x;
	temp.y = lhs - rhs.y;
	return temp;
}

inline eiVector2 operator * (eiScalar lhs, const eiVector2 & rhs)
{
	eiVector2 temp;
	temp.x = lhs * rhs.x;
	temp.y = lhs * rhs.y;
	return temp;
}

inline eiVector2 operator / (eiScalar lhs, const eiVector2 & rhs)
{
	eiVector2 temp;
	temp.x = lhs / rhs.x;
	temp.y = lhs / rhs.y;
	return temp;
}

inline eiVector2 ei_vector2(eiScalar rhs)
{
	eiVector2 temp;
	temp.x = rhs;
	temp.y = rhs;
	return temp;
}

inline eiVector2 ei_vector2(eiScalar x, eiScalar y)
{
	eiVector2 temp;
	temp.x = x;
	temp.y = y;
	return temp;
}

inline eiBool almost_zero(const eiVector2 & a, eiScalar eps)
{
	return (almost_zero(a.x, eps) && almost_zero(a.y, eps));
}

inline eiBool almost_equal(const eiVector2 & a, const eiVector2 & b, eiScalar eps)
{
	return (almost_equal(a.x, b.x, eps) && almost_equal(a.y, b.y, eps));
}

inline eiScalar dot(const eiVector2 & a, const eiVector2 & b)
{
	return (a.x * b.x + a.y * b.y);
}

inline eiScalar len(const eiVector2 & a)
{
	const eiScalar l = dot(a, a);
	return sqrtf(MAX(0.0f, l));
}

inline eiScalar lensq(const eiVector2 & a)
{
	return dot(a, a);
}

inline eiScalar inv_len(const eiVector2 & a)
{
	return invsqrtf(dot(a, a));
}

inline eiScalar dist(const eiVector2 & a, const eiVector2 & b)
{
	return len(b - a);
}

inline eiScalar distsq(const eiVector2 & a, const eiVector2 & b)
{
	const eiVector2 c = b - a;
	return dot(c, c);
}

inline eiVector2 normalize(const eiVector2 & a)
{
	return a * invsqrtf(dot(a, a));
}

#endif
