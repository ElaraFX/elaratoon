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

#ifndef EI_BBOX_H
#define EI_BBOX_H

#include <ei_util.h>
#include <ei_vector.h>

struct eiBBox {

	eiVector min, max;

	inline eiBBox operator + (const eiBBox & rhs) const
	{
		eiBBox temp;
		temp.min.x = min.x + rhs.min.x;
		temp.min.y = min.y + rhs.min.y;
		temp.min.z = min.z + rhs.min.z;
		temp.max.x = max.x + rhs.max.x;
		temp.max.y = max.y + rhs.max.y;
		temp.max.z = max.z + rhs.max.z;
		return temp;
	}

	inline eiBBox operator - (const eiBBox & rhs) const
	{
		eiBBox temp;
		temp.min.x = min.x - rhs.min.x;
		temp.min.y = min.y - rhs.min.y;
		temp.min.z = min.z - rhs.min.z;
		temp.max.x = max.x - rhs.max.x;
		temp.max.y = max.y - rhs.max.y;
		temp.max.z = max.z - rhs.max.z;
		return temp;
	}

	inline eiBBox operator + (eiScalar rhs) const
	{
		eiBBox temp;
		temp.min.x = min.x - rhs;
		temp.min.y = min.y - rhs;
		temp.min.z = min.z - rhs;
		temp.max.x = max.x + rhs;
		temp.max.y = max.y + rhs;
		temp.max.z = max.z + rhs;
		return temp;
	}

	inline eiBBox & operator += (eiScalar rhs)
	{
		min.x -= rhs;
		min.y -= rhs;
		min.z -= rhs;
		max.x += rhs;
		max.y += rhs;
		max.z += rhs;
		return *this;
	}

	inline eiBBox operator * (eiScalar rhs) const
	{
		eiBBox temp;
		temp.min.x = min.x * rhs;
		temp.min.y = min.y * rhs;
		temp.min.z = min.z * rhs;
		temp.max.x = max.x * rhs;
		temp.max.y = max.y * rhs;
		temp.max.z = max.z * rhs;
		return temp;
	}

	inline eiBBox & operator *= (eiScalar rhs)
	{
		min.x *= rhs;
		min.y *= rhs;
		min.z *= rhs;
		max.x *= rhs;
		max.y *= rhs;
		max.z *= rhs;
		return *this;
	}

	inline eiBBox operator / (eiScalar rhs) const
	{
		eiBBox temp;
		const eiScalar inv = rcpf(rhs);
		temp.min.x = min.x * inv;
		temp.min.y = min.y * inv;
		temp.min.z = min.z * inv;
		temp.max.x = max.x * inv;
		temp.max.y = max.y * inv;
		temp.max.z = max.z * inv;
		return temp;
	}

	inline eiBBox & operator /= (eiScalar rhs)
	{
		const eiScalar inv = rcpf(rhs);
		min.x *= inv;
		min.y *= inv;
		min.z *= inv;
		max.x *= inv;
		max.y *= inv;
		max.z *= inv;
		return *this;
	}

	inline bool operator == (const eiBBox & rhs) const
	{
		return (
			min.x == rhs.min.x && min.y == rhs.min.y && min.z == rhs.min.z && 
			max.x == rhs.max.x && max.y == rhs.max.y && max.z == rhs.max.z);
	}

	inline bool operator != (const eiBBox & rhs) const
	{
		return !(*this == rhs);
	}

	inline eiBool is_valid() const
	{
		if (!(min.x <= max.x))
		{
			return EI_FALSE;
		}
		if (!(min.y <= max.y))
		{
			return EI_FALSE;
		}
		if (!(min.z <= max.z))
		{
			return EI_FALSE;
		}
		return EI_TRUE;
	}

	inline eiBool contains(const eiVector & v) const
	{
		return (
			v.x >= min.x && v.x <= max.x && 
			v.y >= min.y && v.y <= max.y && 
			v.z >= min.z && v.z <= max.z);
	}

	inline eiBool contains(const eiBBox & b) const
	{
		return (
			b.min.x >= min.x && b.max.x <= max.x && 
			b.min.y >= min.y && b.max.y <= max.y && 
			b.min.z >= min.z && b.max.z <= max.z);
	}

	inline void diag(eiVector & r) const
	{
		r.x = max.x - min.x;
		r.y = max.y - min.y;
		r.z = max.z - min.z;
	}

	inline eiScalar diag2() const
	{
		eiVector d;

		diag(d);

		return dot(d, d);
	}

	inline void center(eiVector & r) const
	{
		r.x = (min.x + max.x) * 0.5f;
		r.y = (min.y + max.y) * 0.5f;
		r.z = (min.z + max.z) * 0.5f;
	}

	inline eiScalar area() const
	{
		eiVector d;

		if (!is_valid()) /* for empty bounding box, the area is zero */
		{
			return 0.0f;
		}

		diag(d);

		return ((d.x * d.y + d.y * d.z + d.z * d.x) * 2.0f);
	}

	inline eiScalar halfarea() const
	{
		eiVector d;

		if (!is_valid()) /* for empty bounding box, the area is zero */
		{
			return 0.0f;
		}

		diag(d);

		return (d.x * d.y + d.y * d.z + d.z * d.x);
	}

	inline eiScalar volume() const
	{
		eiVector d;

		diag(d);

		return (d.x * d.y * d.z);
	}

	inline eiInt max_axis() const
	{
		eiVector d;
		
		diag(d);

		return d.max_axis();
	}

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		min = 0.0f;
		max = 0.0f;
	}

	inline void AddWithWeight(eiBBox const & src, float weight)
	{
		min += weight * src.min;
		max += weight * src.max;
	}
};

inline eiBBox operator - (const eiBBox & rhs)
{
	eiBBox temp;
	temp.min.x = -rhs.min.x;
	temp.min.y = -rhs.min.y;
	temp.min.z = -rhs.min.z;
	temp.max.x = -rhs.max.x;
	temp.max.y = -rhs.max.y;
	temp.max.z = -rhs.max.z;
	return temp;
}

inline eiBBox operator + (eiScalar lhs, const eiBBox & rhs)
{
	eiBBox temp;
	temp.min.x = rhs.min.x - lhs;
	temp.min.y = rhs.min.y - lhs;
	temp.min.z = rhs.min.z - lhs;
	temp.max.x = rhs.max.x + lhs;
	temp.max.y = rhs.max.y + lhs;
	temp.max.z = rhs.max.z + lhs;
	return temp;
}

inline eiBBox operator * (eiScalar lhs, const eiBBox & rhs)
{
	eiBBox temp;
	temp.min.x = lhs * rhs.min.x;
	temp.min.y = lhs * rhs.min.y;
	temp.min.z = lhs * rhs.min.z;
	temp.max.x = lhs * rhs.max.x;
	temp.max.y = lhs * rhs.max.y;
	temp.max.z = lhs * rhs.max.z;
	return temp;
}

inline eiBBox ei_bbox()
{
	eiBBox temp;
	temp.min.x = EI_BIG_SCALAR;
	temp.min.y = EI_BIG_SCALAR;
	temp.min.z = EI_BIG_SCALAR;
	temp.max.x = -EI_BIG_SCALAR;
	temp.max.y = -EI_BIG_SCALAR;
	temp.max.z = -EI_BIG_SCALAR;
	return temp;
}

inline eiBBox ei_bbox(
	eiScalar xmin, eiScalar xmax, 
	eiScalar ymin, eiScalar ymax, 
	eiScalar zmin, eiScalar zmax)
{
	eiBBox temp;
	temp.min.x = xmin;
	temp.min.y = ymin;
	temp.min.z = zmin;
	temp.max.x = xmax;
	temp.max.y = ymax;
	temp.max.z = zmax;
	return temp;
}

inline void addb(eiBBox & r, const eiBBox & b)
{
	if (r.min.x > b.min.x) {
		r.min.x = b.min.x;
	}
	if (r.max.x < b.max.x) {
		r.max.x = b.max.x;
	}
	if (r.min.y > b.min.y) {
		r.min.y = b.min.y;
	}
	if (r.max.y < b.max.y) {
		r.max.y = b.max.y;
	}
	if (r.min.z > b.min.z) {
		r.min.z = b.min.z;
	}
	if (r.max.z < b.max.z) {
		r.max.z = b.max.z;
	}
}

inline void addbv(eiBBox & r, const eiVector & v)
{
	if (r.min.x > v.x) {
		r.min.x = v.x;
	}
	if (r.max.x < v.x) {
		r.max.x = v.x;
	}
	if (r.min.y > v.y) {
		r.min.y = v.y;
	}
	if (r.max.y < v.y) {
		r.max.y = v.y;
	}
	if (r.min.z > v.z) {
		r.min.z = v.z;
	}
	if (r.max.z < v.z) {
		r.max.z = v.z;
	}
}

inline void fixbf(eiBBox & r, eiScalar v)
{
	eiScalar	temp;
	eiScalar	v2;

	/* make sure max >= min */
	if (r.max.x < r.min.x)
	{
		temp = r.min.x;
		r.min.x = r.max.x;
		r.max.x = temp;
	}

	if (r.max.y < r.min.y)
	{
		temp = r.min.y;
		r.min.y = r.max.y;
		r.max.y = temp;
	}

	if (r.max.z < r.min.z)
	{
		temp = r.min.z;
		r.min.z = r.max.z;
		r.max.z = temp;
	}

	/* make sure no box of zero volume */
	v2 = v * 2.0f;

	if (r.max.x - r.min.x < v2)
	{
		r.min.x -= v;
		r.max.x += v;
	}

	if (r.max.y - r.min.y < v2)
	{
		r.min.y -= v;
		r.max.y += v;
	}

	if (r.max.z - r.min.z < v2)
	{
		r.min.z -= v;
		r.max.z += v;
	}
}

inline void setbv(eiBBox & r, const eiVector & v)
{
	r.min.x = r.max.x = v.x;
	r.min.y = r.max.y = v.y;
	r.min.z = r.max.z = v.z;
}

inline eiBool is_hit_3d(const eiBBox & a, const eiBBox & b, eiBBox & hit)
{
	if (is_hit_1d(a.max.x, a.min.x, b.max.x, b.min.x, hit.max.x, hit.min.x) && 
		is_hit_1d(a.max.y, a.min.y, b.max.y, b.min.y, hit.max.y, hit.min.y) && 
		is_hit_1d(a.max.z, a.min.z, b.max.z, b.min.z, hit.max.z, hit.min.z)) {
		return EI_TRUE;
	} else {
		return EI_FALSE;
	}
}

inline eiBool test_hit_3d(const eiBBox & a, const eiBBox & b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x || 
		a.max.y < b.min.y || a.min.y > b.max.y || 
		a.max.z < b.min.z || a.min.z > b.max.z) {
		return EI_FALSE;
	} else {
		return EI_TRUE;
	}
}

#endif
