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

#ifndef EI_MATRIX_H
#define EI_MATRIX_H

#include <ei_util.h>
#include <ei_vector.h>
#include <ei_bbox.h>

struct eiMatrix {

	eiScalar m[4][4];

	inline eiScalar & operator[] (unsigned int i)
	{
		return *(m[0] + i);
	}
	
	inline eiScalar operator[] (unsigned int i) const
	{
		return *(m[0] + i);
	}

	inline eiMatrix & operator = (eiScalar rhs)
	{
		m[0][0] = rhs;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;
		m[1][0] = 0.0f;
		m[1][1] = rhs;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;
		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = rhs;
		m[2][3] = 0.0f;
		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = rhs;
		return *this;
	}

	inline eiMatrix operator + (const eiMatrix & rhs) const
	{
		eiMatrix temp;
		temp.m[0][0] = m[0][0] + rhs.m[0][0];
		temp.m[0][1] = m[0][1] + rhs.m[0][1];
		temp.m[0][2] = m[0][2] + rhs.m[0][2];
		temp.m[0][3] = m[0][3] + rhs.m[0][3];
		temp.m[1][0] = m[1][0] + rhs.m[1][0];
		temp.m[1][1] = m[1][1] + rhs.m[1][1];
		temp.m[1][2] = m[1][2] + rhs.m[1][2];
		temp.m[1][3] = m[1][3] + rhs.m[1][3];
		temp.m[2][0] = m[2][0] + rhs.m[2][0];
		temp.m[2][1] = m[2][1] + rhs.m[2][1];
		temp.m[2][2] = m[2][2] + rhs.m[2][2];
		temp.m[2][3] = m[2][3] + rhs.m[2][3];
		temp.m[3][0] = m[3][0] + rhs.m[3][0];
		temp.m[3][1] = m[3][1] + rhs.m[3][1];
		temp.m[3][2] = m[3][2] + rhs.m[3][2];
		temp.m[3][3] = m[3][3] + rhs.m[3][3];
		return temp;
	}

	inline eiMatrix & operator += (const eiMatrix & rhs)
	{
		m[0][0] += rhs.m[0][0];
		m[0][1] += rhs.m[0][1];
		m[0][2] += rhs.m[0][2];
		m[0][3] += rhs.m[0][3];
		m[1][0] += rhs.m[1][0];
		m[1][1] += rhs.m[1][1];
		m[1][2] += rhs.m[1][2];
		m[1][3] += rhs.m[1][3];
		m[2][0] += rhs.m[2][0];
		m[2][1] += rhs.m[2][1];
		m[2][2] += rhs.m[2][2];
		m[2][3] += rhs.m[2][3];
		m[3][0] += rhs.m[3][0];
		m[3][1] += rhs.m[3][1];
		m[3][2] += rhs.m[3][2];
		m[3][3] += rhs.m[3][3];
		return *this;
	}

	inline eiMatrix operator - (const eiMatrix & rhs) const
	{
		eiMatrix temp;
		temp.m[0][0] = m[0][0] - rhs.m[0][0];
		temp.m[0][1] = m[0][1] - rhs.m[0][1];
		temp.m[0][2] = m[0][2] - rhs.m[0][2];
		temp.m[0][3] = m[0][3] - rhs.m[0][3];
		temp.m[1][0] = m[1][0] - rhs.m[1][0];
		temp.m[1][1] = m[1][1] - rhs.m[1][1];
		temp.m[1][2] = m[1][2] - rhs.m[1][2];
		temp.m[1][3] = m[1][3] - rhs.m[1][3];
		temp.m[2][0] = m[2][0] - rhs.m[2][0];
		temp.m[2][1] = m[2][1] - rhs.m[2][1];
		temp.m[2][2] = m[2][2] - rhs.m[2][2];
		temp.m[2][3] = m[2][3] - rhs.m[2][3];
		temp.m[3][0] = m[3][0] - rhs.m[3][0];
		temp.m[3][1] = m[3][1] - rhs.m[3][1];
		temp.m[3][2] = m[3][2] - rhs.m[3][2];
		temp.m[3][3] = m[3][3] - rhs.m[3][3];
		return temp;
	}

	inline eiMatrix & operator -= (const eiMatrix & rhs)
	{
		m[0][0] -= rhs.m[0][0];
		m[0][1] -= rhs.m[0][1];
		m[0][2] -= rhs.m[0][2];
		m[0][3] -= rhs.m[0][3];
		m[1][0] -= rhs.m[1][0];
		m[1][1] -= rhs.m[1][1];
		m[1][2] -= rhs.m[1][2];
		m[1][3] -= rhs.m[1][3];
		m[2][0] -= rhs.m[2][0];
		m[2][1] -= rhs.m[2][1];
		m[2][2] -= rhs.m[2][2];
		m[2][3] -= rhs.m[2][3];
		m[3][0] -= rhs.m[3][0];
		m[3][1] -= rhs.m[3][1];
		m[3][2] -= rhs.m[3][2];
		m[3][3] -= rhs.m[3][3];
		return *this;
	}

	inline eiMatrix operator * (const eiMatrix & rhs) const
	{
		eiMatrix temp;
		temp.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
		temp.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
		temp.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
		temp.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];
		temp.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
		temp.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
		temp.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
		temp.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];
		temp.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
		temp.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
		temp.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
		temp.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];
		temp.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
		temp.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
		temp.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
		temp.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
		return temp;
	}

	inline eiMatrix & operator *= (const eiMatrix & rhs)
	{
		eiMatrix temp;
		temp.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
		temp.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
		temp.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
		temp.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];
		temp.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
		temp.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
		temp.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
		temp.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];
		temp.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
		temp.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
		temp.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
		temp.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];
		temp.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
		temp.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
		temp.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
		temp.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
		*this = temp;
		return *this;
	}

	inline eiMatrix operator * (eiScalar rhs) const
	{
		eiMatrix temp;
		temp.m[0][0] = m[0][0] * rhs;
		temp.m[0][1] = m[0][1] * rhs;
		temp.m[0][2] = m[0][2] * rhs;
		temp.m[0][3] = m[0][3] * rhs;
		temp.m[1][0] = m[1][0] * rhs;
		temp.m[1][1] = m[1][1] * rhs;
		temp.m[1][2] = m[1][2] * rhs;
		temp.m[1][3] = m[1][3] * rhs;
		temp.m[2][0] = m[2][0] * rhs;
		temp.m[2][1] = m[2][1] * rhs;
		temp.m[2][2] = m[2][2] * rhs;
		temp.m[2][3] = m[2][3] * rhs;
		temp.m[3][0] = m[3][0] * rhs;
		temp.m[3][1] = m[3][1] * rhs;
		temp.m[3][2] = m[3][2] * rhs;
		temp.m[3][3] = m[3][3] * rhs;
		return temp;
	}

	inline eiMatrix & operator *= (eiScalar rhs)
	{
		m[0][0] *= rhs;
		m[0][1] *= rhs;
		m[0][2] *= rhs;
		m[0][3] *= rhs;
		m[1][0] *= rhs;
		m[1][1] *= rhs;
		m[1][2] *= rhs;
		m[1][3] *= rhs;
		m[2][0] *= rhs;
		m[2][1] *= rhs;
		m[2][2] *= rhs;
		m[2][3] *= rhs;
		m[3][0] *= rhs;
		m[3][1] *= rhs;
		m[3][2] *= rhs;
		m[3][3] *= rhs;
		return *this;
	}

	inline eiMatrix operator / (eiScalar rhs) const
	{
		eiMatrix temp;
		const eiScalar inv = rcpf(rhs);
		temp.m[0][0] = m[0][0] * inv;
		temp.m[0][1] = m[0][1] * inv;
		temp.m[0][2] = m[0][2] * inv;
		temp.m[0][3] = m[0][3] * inv;
		temp.m[1][0] = m[1][0] * inv;
		temp.m[1][1] = m[1][1] * inv;
		temp.m[1][2] = m[1][2] * inv;
		temp.m[1][3] = m[1][3] * inv;
		temp.m[2][0] = m[2][0] * inv;
		temp.m[2][1] = m[2][1] * inv;
		temp.m[2][2] = m[2][2] * inv;
		temp.m[2][3] = m[2][3] * inv;
		temp.m[3][0] = m[3][0] * inv;
		temp.m[3][1] = m[3][1] * inv;
		temp.m[3][2] = m[3][2] * inv;
		temp.m[3][3] = m[3][3] * inv;
		return temp;
	}

	inline eiMatrix & operator /= (eiScalar rhs)
	{
		const eiScalar inv = rcpf(rhs);
		m[0][0] *= inv;
		m[0][1] *= inv;
		m[0][2] *= inv;
		m[0][3] *= inv;
		m[1][0] *= inv;
		m[1][1] *= inv;
		m[1][2] *= inv;
		m[1][3] *= inv;
		m[2][0] *= inv;
		m[2][1] *= inv;
		m[2][2] *= inv;
		m[2][3] *= inv;
		m[3][0] *= inv;
		m[3][1] *= inv;
		m[3][2] *= inv;
		m[3][3] *= inv;
		return *this;
	}

	inline bool operator == (const eiMatrix & rhs) const
	{
		return (
			m[0][0] == rhs.m[0][0] && 
			m[0][1] == rhs.m[0][1] && 
			m[0][2] == rhs.m[0][2] && 
			m[0][3] == rhs.m[0][3] && 
			m[1][0] == rhs.m[1][0] && 
			m[1][1] == rhs.m[1][1] && 
			m[1][2] == rhs.m[1][2] && 
			m[1][3] == rhs.m[1][3] && 
			m[2][0] == rhs.m[2][0] && 
			m[2][1] == rhs.m[2][1] && 
			m[2][2] == rhs.m[2][2] && 
			m[2][3] == rhs.m[2][3] && 
			m[3][0] == rhs.m[3][0] && 
			m[3][1] == rhs.m[3][1] && 
			m[3][2] == rhs.m[3][2] && 
			m[3][3] == rhs.m[3][3]);
	}

	inline bool operator != (const eiMatrix & rhs) const
	{
		return !(*this == rhs);
	}

	inline eiVector get_translation() const
	{
		return ei_vector(m[3][0], m[3][1], m[3][2]);
	}

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		(*this) = 0.0f;
	}

	inline void AddWithWeight(eiMatrix const & src, float weight)
	{
		(*this) += src * weight;
	}
};

inline eiMatrix operator - (const eiMatrix & rhs)
{
	eiMatrix temp;
	temp.m[0][0] = -rhs.m[0][0];
	temp.m[0][1] = -rhs.m[0][1];
	temp.m[0][2] = -rhs.m[0][2];
	temp.m[0][3] = -rhs.m[0][3];
	temp.m[1][0] = -rhs.m[1][0];
	temp.m[1][1] = -rhs.m[1][1];
	temp.m[1][2] = -rhs.m[1][2];
	temp.m[1][3] = -rhs.m[1][3];
	temp.m[2][0] = -rhs.m[2][0];
	temp.m[2][1] = -rhs.m[2][1];
	temp.m[2][2] = -rhs.m[2][2];
	temp.m[2][3] = -rhs.m[2][3];
	temp.m[3][0] = -rhs.m[3][0];
	temp.m[3][1] = -rhs.m[3][1];
	temp.m[3][2] = -rhs.m[3][2];
	temp.m[3][3] = -rhs.m[3][3];
	return temp;
}

inline eiMatrix operator * (eiScalar lhs, const eiMatrix & rhs)
{
	eiMatrix temp;
	temp.m[0][0] = lhs * rhs.m[0][0];
	temp.m[0][1] = lhs * rhs.m[0][1];
	temp.m[0][2] = lhs * rhs.m[0][2];
	temp.m[0][3] = lhs * rhs.m[0][3];
	temp.m[1][0] = lhs * rhs.m[1][0];
	temp.m[1][1] = lhs * rhs.m[1][1];
	temp.m[1][2] = lhs * rhs.m[1][2];
	temp.m[1][3] = lhs * rhs.m[1][3];
	temp.m[2][0] = lhs * rhs.m[2][0];
	temp.m[2][1] = lhs * rhs.m[2][1];
	temp.m[2][2] = lhs * rhs.m[2][2];
	temp.m[2][3] = lhs * rhs.m[2][3];
	temp.m[3][0] = lhs * rhs.m[3][0];
	temp.m[3][1] = lhs * rhs.m[3][1];
	temp.m[3][2] = lhs * rhs.m[3][2];
	temp.m[3][3] = lhs * rhs.m[3][3];
	return temp;
}

inline eiMatrix ei_matrix(
	eiScalar m00, eiScalar m01, eiScalar m02, eiScalar m03, 
	eiScalar m10, eiScalar m11, eiScalar m12, eiScalar m13, 
	eiScalar m20, eiScalar m21, eiScalar m22, eiScalar m23, 
	eiScalar m30, eiScalar m31, eiScalar m32, eiScalar m33)
{
	eiMatrix temp;
	temp.m[0][0] = m00;
	temp.m[0][1] = m01;
	temp.m[0][2] = m02;
	temp.m[0][3] = m03;
	temp.m[1][0] = m10;
	temp.m[1][1] = m11;
	temp.m[1][2] = m12;
	temp.m[1][3] = m13;
	temp.m[2][0] = m20;
	temp.m[2][1] = m21;
	temp.m[2][2] = m22;
	temp.m[2][3] = m23;
	temp.m[3][0] = m30;
	temp.m[3][1] = m31;
	temp.m[3][2] = m32;
	temp.m[3][3] = m33;
	return temp;
}

inline eiBool almost_equal(const eiMatrix & a, const eiMatrix & b, eiScalar zero)
{
	return (
		almost_equal(a.m[0][0], b.m[0][0], zero) && 
		almost_equal(a.m[0][1], b.m[0][1], zero) && 
		almost_equal(a.m[0][2], b.m[0][2], zero) && 
		almost_equal(a.m[0][3], b.m[0][3], zero) && 
		almost_equal(a.m[1][0], b.m[1][0], zero) && 
		almost_equal(a.m[1][1], b.m[1][1], zero) && 
		almost_equal(a.m[1][2], b.m[1][2], zero) && 
		almost_equal(a.m[1][3], b.m[1][3], zero) && 
		almost_equal(a.m[2][0], b.m[2][0], zero) && 
		almost_equal(a.m[2][1], b.m[2][1], zero) && 
		almost_equal(a.m[2][2], b.m[2][2], zero) && 
		almost_equal(a.m[2][3], b.m[2][3], zero) && 
		almost_equal(a.m[3][0], b.m[3][0], zero) && 
		almost_equal(a.m[3][1], b.m[3][1], zero) && 
		almost_equal(a.m[3][2], b.m[3][2], zero) && 
		almost_equal(a.m[3][3], b.m[3][3], zero));
}

inline eiScalar determinant(const eiMatrix & rhs)
{
	return (
		rhs.m[0][3] * rhs.m[1][2] * rhs.m[2][1] * rhs.m[3][0] - rhs.m[0][2] * rhs.m[1][3] * rhs.m[2][1] * rhs.m[3][0] - rhs.m[0][3] * rhs.m[1][1] * rhs.m[2][2] * rhs.m[3][0] + rhs.m[0][1] * rhs.m[1][3] * rhs.m[2][2] * rhs.m[3][0] + 
		rhs.m[0][2] * rhs.m[1][1] * rhs.m[2][3] * rhs.m[3][0] - rhs.m[0][1] * rhs.m[1][2] * rhs.m[2][3] * rhs.m[3][0] - rhs.m[0][3] * rhs.m[1][2] * rhs.m[2][0] * rhs.m[3][1] + rhs.m[0][2] * rhs.m[1][3] * rhs.m[2][0] * rhs.m[3][1] + 
		rhs.m[0][3] * rhs.m[1][0] * rhs.m[2][2] * rhs.m[3][1] - rhs.m[0][0] * rhs.m[1][3] * rhs.m[2][2] * rhs.m[3][1] - rhs.m[0][2] * rhs.m[1][0] * rhs.m[2][3] * rhs.m[3][1] + rhs.m[0][0] * rhs.m[1][2] * rhs.m[2][3] * rhs.m[3][1] + 
		rhs.m[0][3] * rhs.m[1][1] * rhs.m[2][0] * rhs.m[3][2] - rhs.m[0][1] * rhs.m[1][3] * rhs.m[2][0] * rhs.m[3][2] - rhs.m[0][3] * rhs.m[1][0] * rhs.m[2][1] * rhs.m[3][2] + rhs.m[0][0] * rhs.m[1][3] * rhs.m[2][1] * rhs.m[3][2] + 
		rhs.m[0][1] * rhs.m[1][0] * rhs.m[2][3] * rhs.m[3][2] - rhs.m[0][0] * rhs.m[1][1] * rhs.m[2][3] * rhs.m[3][2] - rhs.m[0][2] * rhs.m[1][1] * rhs.m[2][0] * rhs.m[3][3] + rhs.m[0][1] * rhs.m[1][2] * rhs.m[2][0] * rhs.m[3][3] + 
		rhs.m[0][2] * rhs.m[1][0] * rhs.m[2][1] * rhs.m[3][3] - rhs.m[0][0] * rhs.m[1][2] * rhs.m[2][1] * rhs.m[3][3] - rhs.m[0][1] * rhs.m[1][0] * rhs.m[2][2] * rhs.m[3][3] + rhs.m[0][0] * rhs.m[1][1] * rhs.m[2][2] * rhs.m[3][3]);
}

/** Code below from:
 * ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf
 */
inline eiMatrix inverse(const eiMatrix & rhs)
{
	eiMatrix temp;

	eiScalar *dst = &temp.m[0][0];
	const eiScalar *mat = &rhs.m[0][0];

	eiScalar tmp[12];	/* temp array for pairs */
	eiScalar src[16];	/* array of transpose source matrix */
	eiScalar det;		/* determinant */

#define TRANSPOSE_ROW(i) \
	src[i]		= mat[i*4];\
	src[i + 4]	= mat[i*4 + 1];\
	src[i + 8]	= mat[i*4 + 2];\
	src[i + 12]	= mat[i*4 + 3];

	/* transpose matrix */
	TRANSPOSE_ROW(0);
	TRANSPOSE_ROW(1);
	TRANSPOSE_ROW(2);
	TRANSPOSE_ROW(3);

	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0]	= src[10]	* src[15];
	tmp[1]	= src[11]	* src[14];
	tmp[2]	= src[9]	* src[15];
	tmp[3]	= src[11]	* src[13];
	tmp[4]	= src[9]	* src[14];
	tmp[5]	= src[10]	* src[13];
	tmp[6]	= src[8]	* src[15];
	tmp[7]	= src[11]	* src[12];
	tmp[8]	= src[8]	* src[14];
	tmp[9]	= src[10]	* src[12];
	tmp[10]	= src[8]	* src[13];
	tmp[11]	= src[9]	* src[12];

	/* calculate first 8 elements (cofactors) */
	dst[0]	 = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0]	-= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1]	 = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1]	-= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2]	 = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2]	-= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3]	 = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3]	-= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4]	 = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4]	-= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5]	 = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5]	-= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6]	 = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6]	-= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7]	 = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7]	-= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0]	= src[2]*src[7];
	tmp[1]	= src[3]*src[6];
	tmp[2]	= src[1]*src[7];
	tmp[3]	= src[3]*src[5];
	tmp[4]	= src[1]*src[6];
	tmp[5]	= src[2]*src[5];
	tmp[6]	= src[0]*src[7];
	tmp[7]	= src[3]*src[4];
	tmp[8]	= src[0]*src[6];
	tmp[9]	= src[2]*src[4];
	tmp[10]	= src[0]*src[5];
	tmp[11]	= src[1]*src[4];

	/* calculate second 8 elements (cofactors) */
	dst[8]	 = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8]	-= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9]	 = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9]	-= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10]	 = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]	-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11]	 = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]	-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12]	 = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]	-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13]	 = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]	-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14]	 = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]	-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15]	 = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]	-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

	/* calculate determinant */
	det = src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];

	/* calculate matrix inverse */
	det = (det != 0.0f) ? rcpf(det) : 1.0f;
	dst[0] *= det;
	dst[1] *= det;
	dst[2] *= det;
	dst[3] *= det;
	dst[4] *= det;
	dst[5] *= det;
	dst[6] *= det;
	dst[7] *= det;
	dst[8] *= det;
	dst[9] *= det;
	dst[10] *= det;
	dst[11] *= det;
	dst[12] *= det;
	dst[13] *= det;
	dst[14] *= det;
	dst[15] *= det;

	return temp;
}

inline eiMatrix transpose(const eiMatrix & rhs)
{
	eiMatrix temp;
	temp.m[0][0] = rhs.m[0][0];
	temp.m[0][1] = rhs.m[1][0];
	temp.m[0][2] = rhs.m[2][0];
	temp.m[0][3] = rhs.m[3][0];
	temp.m[1][0] = rhs.m[0][1];
	temp.m[1][1] = rhs.m[1][1];
	temp.m[1][2] = rhs.m[2][1];
	temp.m[1][3] = rhs.m[3][1];
	temp.m[2][0] = rhs.m[0][2];
	temp.m[2][1] = rhs.m[1][2];
	temp.m[2][2] = rhs.m[2][2];
	temp.m[2][3] = rhs.m[3][2];
	temp.m[3][0] = rhs.m[0][3];
	temp.m[3][1] = rhs.m[1][3];
	temp.m[3][2] = rhs.m[2][3];
	temp.m[3][3] = rhs.m[3][3];
	return temp;
}

inline eiVector point_transform(const eiVector & vec, const eiMatrix & mx)
{
	eiVector temp;
	temp.x = vec.x * mx.m[0][0] + vec.y * mx.m[1][0] + vec.z * mx.m[2][0] + mx.m[3][0];
	temp.y = vec.x * mx.m[0][1] + vec.y * mx.m[1][1] + vec.z * mx.m[2][1] + mx.m[3][1];
	temp.z = vec.x * mx.m[0][2] + vec.y * mx.m[1][2] + vec.z * mx.m[2][2] + mx.m[3][2];
	return temp;
}

inline eiVector motion_point_transform(const eiVector & vec, const eiMatrix & mx1, const eiMatrix & mx2, eiScalar t)
{
	const eiVector rv1 = point_transform(vec, mx1);
	const eiVector rv2 = point_transform(vec, mx2);
	
	return lerp(rv1, rv2, t);
}

inline eiVector lerp_motion_point_transform(const eiVector & v1, const eiVector & v2, const eiMatrix & mx1, const eiMatrix & mx2, eiScalar t)
{
	const eiVector rv1 = point_transform(v1, mx1);
	const eiVector rv2 = point_transform(v2, mx2);

	return lerp(rv1, rv2, t);
}

inline eiVector vector_transform(const eiVector & vec, const eiMatrix & mx)
{
	eiVector temp;
	temp.x = vec.x * mx.m[0][0] + vec.y * mx.m[1][0] + vec.z * mx.m[2][0];
	temp.y = vec.x * mx.m[0][1] + vec.y * mx.m[1][1] + vec.z * mx.m[2][1];
	temp.z = vec.x * mx.m[0][2] + vec.y * mx.m[1][2] + vec.z * mx.m[2][2];
	return temp;
}

inline eiVector motion_vector_transform(const eiVector & vec, const eiMatrix & mx1, const eiMatrix & mx2, eiScalar t)
{
	eiVector rv1, rv2;

	rv1 = vector_transform(vec, mx1);
	rv2 = vector_transform(vec, mx2);

	return lerp(rv1, rv2, t);
}

inline eiVector transpose_vector_transform(const eiVector & vec, const eiMatrix & mx)
{
	eiVector temp;
	temp.x = vec.x * mx.m[0][0] + vec.y * mx.m[0][1] + vec.z * mx.m[0][2];
	temp.y = vec.x * mx.m[1][0] + vec.y * mx.m[1][1] + vec.z * mx.m[1][2];
	temp.z = vec.x * mx.m[2][0] + vec.y * mx.m[2][1] + vec.z * mx.m[2][2];
	return temp;
}

inline eiVector motion_transpose_vector_transform(const eiVector & vec, const eiMatrix & mx1, const eiMatrix & mx2, eiScalar t)
{
	const eiVector rv1 = transpose_vector_transform(vec, mx1);
	const eiVector rv2 = transpose_vector_transform(vec, mx2);
	
	return lerp(rv1, rv2, t);
}

inline void transform_box(eiBBox & t_box, const eiBBox & box, const eiMatrix & transform)
{
	eiVector pos, vx;

	pos.x = box.min.x;
	pos.y = box.min.y;
	pos.z = box.min.z;
	vx = point_transform(pos, transform);
	setbv(t_box, vx);

	pos.x = box.min.x;
	pos.y = box.min.y;
	pos.z = box.max.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.min.x;
	pos.y = box.max.y;
	pos.z = box.min.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.min.x;
	pos.y = box.max.y;
	pos.z = box.max.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.max.x;
	pos.y = box.min.y;
	pos.z = box.min.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.max.x;
	pos.y = box.min.y;
	pos.z = box.max.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.max.x;
	pos.y = box.max.y;
	pos.z = box.min.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);

	pos.x = box.max.x;
	pos.y = box.max.y;
	pos.z = box.max.z;
	vx = point_transform(pos, transform);
	addbv(t_box, vx);
}

/** Transform a ray using a matrix.
 */
__forceinline void ei_transform_ray(
	eiVector & dst_org, 
	eiVector & dst_dir, 
	const eiVector & src_org, 
	const eiVector & src_dir, 
	const eiMatrix & transform)
{
	dst_org = point_transform(src_org, transform);
	dst_dir = vector_transform(src_dir, transform);
}

/** Transform a ray using a matrix, 
 * a motion matrix, and a time.
 */
__forceinline void ei_motion_transform_ray(
	eiVector & dst_org, 
	eiVector & dst_dir, 
	const eiVector & src_org, 
	const eiVector & src_dir, 
	const eiMatrix & transform, 
	const eiMatrix & motion_transform, 
	const eiScalar time)
{
	const eiVector org1 = point_transform(src_org, transform);
	const eiVector org2 = point_transform(src_org, motion_transform);
	const eiVector dir1 = vector_transform(src_dir, transform);
	const eiVector dir2 = vector_transform(src_dir, motion_transform);

	dst_org = lerp(org1, org2, time);
	dst_dir = lerp(dir1, dir2, time);
}

inline eiMatrix translate(eiScalar tx, eiScalar ty, eiScalar tz)
{
	return ei_matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		tx, ty, tz, 1.0f);
}

inline eiMatrix scale(eiScalar sx, eiScalar sy, eiScalar sz)
{
	return ei_matrix(
		sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline eiMatrix rotate_x(eiScalar angle)
{
	const eiScalar sine = sin(angle);
	const eiScalar cosine = cos(angle);

	return ei_matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosine, sine, 0.0f,
		0.0f, -sine, cosine, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline eiMatrix rotate_y(eiScalar angle)
{
	const eiScalar sine = sin(angle);
	const eiScalar cosine = cos(angle);

	return ei_matrix(
		cosine, 0.0f, -sine, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sine, 0.0f, cosine, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline eiMatrix rotate_z(eiScalar angle)
{
	const eiScalar sine = sin(angle);
	const eiScalar cosine = cos(angle);

	return ei_matrix(
		cosine, sine, 0.0f, 0.0f,
		-sine, cosine, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline eiMatrix rotate(eiScalar angle, const eiVector & axis)
{
	const eiScalar sn = sin(angle);
	const eiScalar cs = cos(angle);
	const eiScalar one_minus_cs = 1.0f - cs;

	return ei_matrix(
		axis.x * axis.x + (1.0f - axis.x * axis.x) * cs,
		axis.x * axis.y * one_minus_cs + axis.z * sn,
		axis.x * axis.z * one_minus_cs - axis.y * sn,
		0.0f,
		axis.x * axis.y * one_minus_cs - axis.z * sn,
		axis.y * axis.y + (1.0f - axis.y * axis.y) * cs,
		axis.y * axis.z * one_minus_cs + axis.x * sn,
		0.0f,
		axis.x * axis.z * one_minus_cs + axis.y * sn,
		axis.y * axis.z * one_minus_cs - axis.x * sn,
		axis.z * axis.z + (1.0f - axis.z * axis.z) * cs,
		0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline eiBool neg_parity(const eiMatrix & mx)
{
	const eiVector row0 = ei_vector(mx.m[0][0], mx.m[0][1], mx.m[0][2]);
	const eiVector row1 = ei_vector(mx.m[1][0], mx.m[1][1], mx.m[1][2]);
	const eiVector row2 = ei_vector(mx.m[2][0], mx.m[2][1], mx.m[2][2]);

	if (dot(cross(row0, row1), row2) < 0.0f)
	{
		return EI_TRUE;
	}
	else
	{
		return EI_FALSE;
	}
}

inline void remove_scaling(eiMatrix & mx)
{
	eiVector row;
	
	row = normalize(ei_vector(mx.m[0][0], mx.m[0][1], mx.m[0][2]));
	mx.m[0][0] = row.x;
	mx.m[0][1] = row.y;
	mx.m[0][2] = row.z;

	row = normalize(ei_vector(mx.m[1][0], mx.m[1][1], mx.m[1][2]));
	mx.m[1][0] = row.x;
	mx.m[1][1] = row.y;
	mx.m[1][2] = row.z;

	row = normalize(ei_vector(mx.m[2][0], mx.m[2][1], mx.m[2][2]));
	mx.m[2][0] = row.x;
	mx.m[2][1] = row.y;
	mx.m[2][2] = row.z;
}

inline eiScalar get_scaling(const eiMatrix & mx)
{
	eiScalar scale = len(ei_vector(mx.m[0][0], mx.m[0][1], mx.m[0][2]));
	scale = max(scale, len(ei_vector(mx.m[1][0], mx.m[1][1], mx.m[1][2])));
	scale = max(scale, len(ei_vector(mx.m[2][0], mx.m[2][1], mx.m[2][2])));
	return scale;
}

inline eiScalar uniform_scaling(eiMatrix & mx)
{
	eiVector row;

	eiScalar scale = normalize_len(row, ei_vector(mx.m[0][0], mx.m[0][1], mx.m[0][2]));
	mx.m[0][0] = row.x;
	mx.m[0][1] = row.y;
	mx.m[0][2] = row.z;

	scale = max(scale, normalize_len(row, ei_vector(mx.m[1][0], mx.m[1][1], mx.m[1][2])));
	mx.m[1][0] = row.x;
	mx.m[1][1] = row.y;
	mx.m[1][2] = row.z;

	scale = max(scale, normalize_len(row, ei_vector(mx.m[2][0], mx.m[2][1], mx.m[2][2])));
	mx.m[2][0] = row.x;
	mx.m[2][1] = row.y;
	mx.m[2][2] = row.z;

	mx.m[0][0] *= scale;
	mx.m[0][1] *= scale;
	mx.m[0][2] *= scale;
	mx.m[0][3] *= scale;
	mx.m[1][0] *= scale;
	mx.m[1][1] *= scale;
	mx.m[1][2] *= scale;
	mx.m[1][3] *= scale;
	mx.m[2][0] *= scale;
	mx.m[2][1] *= scale;
	mx.m[2][2] *= scale;
	mx.m[2][3] *= scale;

	return scale;
}

static const eiMatrix g_IdentityMatrix = { 
	1.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 1.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 1.0f
};

static const eiMatrix g_ZeroMatrix = {
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 
};

/* OSL interoperability */
#if defined eiCORE_EXPORTS || defined EI_OSL_INTEROP

inline void to_mat44(OSL::Matrix44 & m, const eiMatrix & v)
{
	m.x[0][0] = v.m[0][0];
	m.x[0][1] = v.m[0][1];
	m.x[0][2] = v.m[0][2];
	m.x[0][3] = v.m[0][3];
	m.x[1][0] = v.m[1][0];
	m.x[1][1] = v.m[1][1];
	m.x[1][2] = v.m[1][2];
	m.x[1][3] = v.m[1][3];
	m.x[2][0] = v.m[2][0];
	m.x[2][1] = v.m[2][1];
	m.x[2][2] = v.m[2][2];
	m.x[2][3] = v.m[2][3];
	m.x[3][0] = v.m[3][0];
	m.x[3][1] = v.m[3][1];
	m.x[3][2] = v.m[3][2];
	m.x[3][3] = v.m[3][3];
}

inline void from_mat44(eiMatrix & m, const OSL::Matrix44 & v)
{
	m.m[0][0] = v.x[0][0];
	m.m[0][1] = v.x[0][1];
	m.m[0][2] = v.x[0][2];
	m.m[0][3] = v.x[0][3];
	m.m[1][0] = v.x[1][0];
	m.m[1][1] = v.x[1][1];
	m.m[1][2] = v.x[1][2];
	m.m[1][3] = v.x[1][3];
	m.m[2][0] = v.x[2][0];
	m.m[2][1] = v.x[2][1];
	m.m[2][2] = v.x[2][2];
	m.m[2][3] = v.x[2][3];
	m.m[3][0] = v.x[3][0];
	m.m[3][1] = v.x[3][1];
	m.m[3][2] = v.x[3][2];
	m.m[3][3] = v.x[3][3];
}

/** Interpolatable transform.
 */
class eiTransform
{
public:
	inline eiTransform() : 
		m_moving(EI_FALSE), 
		m_mat0((const OSL::Matrix44 *)(&g_IdentityMatrix)), 
		m_mat1((const OSL::Matrix44 *)(&g_IdentityMatrix)), 
		m_inv_mat0((const OSL::Matrix44 *)(&g_IdentityMatrix)), 
		m_inv_mat1((const OSL::Matrix44 *)(&g_IdentityMatrix))
	{
	}

	inline void set_moving(const eiBool moving)
	{
		m_moving = moving;
	}

	inline void set_matrix(const eiMatrix & mat)
	{
		m_mat0 = (const OSL::Matrix44 *)(&mat);
	}

	inline void set_motion_matrix(const eiMatrix & mat)
	{
		m_mat1 = (const OSL::Matrix44 *)(&mat);
	}

	inline void set_inverse_matrix(const eiMatrix & mat)
	{
		m_inv_mat0 = (const OSL::Matrix44 *)(&mat);
	}

	inline void set_inverse_motion_matrix(const eiMatrix & mat)
	{
		m_inv_mat1 = (const OSL::Matrix44 *)(&mat);
	}

	inline eiBool is_moving() const
	{
		return m_moving;
	}

	inline void get_static(OSL::Matrix44 & result) const
	{
		result = *m_mat0;
	}

	inline void get_motion(OSL::Matrix44 & result, float time) const
	{
		if (!m_moving)
		{
			result = *m_mat0;
			return;
		}

		result = lerp(*m_mat0, *m_mat1, time);
	}

	inline void get_inverse_static(OSL::Matrix44 & result) const
	{
		result = *m_inv_mat0;
	}

	inline void get_inverse_motion(OSL::Matrix44 & result, float time) const
	{
		if (!m_moving)
		{
			result = *m_inv_mat0;
			return;
		}

		result = lerp(*m_inv_mat0, *m_inv_mat1, time);
	}

private:
	eiBool					m_moving;
	const OSL::Matrix44		*m_mat0;
	const OSL::Matrix44		*m_mat1;
	const OSL::Matrix44		*m_inv_mat0;
	const OSL::Matrix44		*m_inv_mat1;
};

#endif

#endif
