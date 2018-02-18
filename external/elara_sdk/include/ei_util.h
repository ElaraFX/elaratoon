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

#ifndef EI_UTIL_H
#define EI_UTIL_H

#include <ei_types.h>
#include <ei_platform.h>

#include <float.h>
#include <math.h>
#include <xmmintrin.h>

#ifndef X_AXIS
#define X_AXIS		0
#define Y_AXIS		1
#define Z_AXIS		2
#endif

#define INV_LOG_10_2	3.32192809488736234787031942948939f /* (1 / log10(2)) */

inline eiScalar log2f(eiScalar x)
{
	return log10f(x) * INV_LOG_10_2;
}

inline eiScalar absf(eiScalar x)
{
	union { float f; eiUint32 i; } v = {x};
	v.i &= 0x7FFFFFFF;
	return v.f;
}

inline eiScalar rcpf(eiScalar x)
{
#ifdef _MSC_VER
	return 1.0f / x;
#else
	const __m128 a = _mm_set_ss(x);
	const __m128 r = _mm_rcp_ss(a);
	/* one more iteration */
	return _mm_cvtss_f32(_mm_sub_ss(_mm_add_ss(r, r), _mm_mul_ss(_mm_mul_ss(r, r), a)));
#endif
}

inline eiScalar safe_rcp(eiScalar x)
{
	if (absf(x) <= EI_NORM_EPS)
	{
		if (x >= 0.0f)
		{
			return EI_BIG_SCALAR;
		}
		else
		{
			return -EI_BIG_SCALAR;
		}
	}
	return rcpf(x);
}

inline eiScalar invsqrtf(eiScalar x)
{
	const __m128 a = _mm_max_ss(_mm_set_ss(x), _mm_set_ss(EI_NORM_EPS));
	const __m128 r = _mm_rsqrt_ss(a);
	/* one more iteration */
	return _mm_cvtss_f32(_mm_mul_ss(r, _mm_add_ss(_mm_set_ss(1.5f), 
		_mm_mul_ss(_mm_mul_ss(a, _mm_set_ss(-0.5f)), _mm_mul_ss(r, r)))));
}

/* Quake 3 fast inverse square root algorithm */
inline eiScalar fastinvsqrt(eiScalar x)
{
	eiScalar xhalf = 0.5f * x;
	eiInt i = *(eiInt *)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(eiScalar *)&i;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

/* Quake 3 fast square root algorithm */
inline eiScalar fastsqrt(eiScalar x)
{
	union {
		eiUint32 intPart;
		eiScalar floatPart;
	} convertor, convertor2;
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1fbcf800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
}

/* Code below are from http://code.google.com/p/fastapprox/ */
inline float fastpow2(float p)
{
	float offset = (p < 0) ? 1.0f : 0.0f;
	float clipp = (p < -126) ? -126.0f : p;
	int w = (int)clipp;
	float z = clipp - w + offset;
	union { eiUint32 i; float f; } v = { (eiUint32)((1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z)) };
	return v.f;
}

inline float fastexp(float p)
{
  return fastpow2(1.442695040f * p);
}

inline float fastlog2(float x)
{
	union { float f; eiUint32 i; } vx = { x };
	union { eiUint32 i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
	float y = (float)vx.i;
	y *= 1.1920928955078125e-7f;
	return y - 124.22551499f 
		- 1.498030302f * mx.f 
		- 1.72587999f / (0.3520887068f + mx.f);
}

inline float fastlog(float x)
{
	return 0.69314718f * fastlog2(x);
}

inline float fastpow(float x, float p)
{
	return fastpow2(p * fastlog2(x));
}

inline float fastsin(float x)
{
	const float fouroverpi = 1.2732395447351627f;
	const float fouroverpisq = 0.40528473456935109f;
	const float q = 0.78444488374548933f;
	union { float f; eiUint32 i; } p = { 0.20363937680730309f };
	union { float f; eiUint32 i; } r = { 0.015124940802184233f };
	union { float f; eiUint32 i; } s = { -0.0032225901625579573f };

	union { float f; eiUint32 i; } vx = { x };
	eiUint32 sign = vx.i & 0x80000000;
	float qpprox;
	float qpproxsq;
	vx.i = vx.i & 0x7FFFFFFF;

	qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
	qpproxsq = qpprox * qpprox;

	p.i |= sign;
	r.i |= sign;
	s.i ^= sign;

	return q * qpprox + qpproxsq * (p.f + qpproxsq * (r.f + qpproxsq * s.f));
}

inline float fastersin(float x)
{
	const float fouroverpi = 1.2732395447351627f;
	const float fouroverpisq = 0.40528473456935109f;
	const float q = 0.77633023248007499f;
	union { float f; eiUint32 i; } p = { 0.22308510060189463f };

	union { float f; eiUint32 i; } vx = { x };
	eiUint32 sign = vx.i & 0x80000000;
	float qpprox;
	vx.i &= 0x7FFFFFFF;

	qpprox = fouroverpi * x - fouroverpisq * x * vx.f;

	p.i |= sign;

	return qpprox * (q + p.f * qpprox);
}

inline float fastsinfull(float x)
{
	const float twopi = 6.2831853071795865f;
	const float invtwopi = 0.15915494309189534f;

	int k = (int)(x * invtwopi);
	float half = (x < 0) ? -0.5f : 0.5f;
	return fastsin ((half + k) * twopi - x);
}

inline float fastersinfull(float x)
{
	const float twopi = 6.2831853071795865f;
	const float invtwopi = 0.15915494309189534f;

	int k = (int)(x * invtwopi);
	float half = (x < 0) ? -0.5f : 0.5f;
	return fastersin ((half + k) * twopi - x);
}

inline float fastcos(float x)
{
	const float halfpi = 1.5707963267948966f;
	const float halfpiminustwopi = -4.7123889803846899f;
	float offset = (x > halfpi) ? halfpiminustwopi : halfpi;
	return fastsin (x + offset);
}

inline float fastercos(float x)
{
	const float twooverpi = 0.63661977236758134f;
	const float p = 0.54641335845679634f;

	union { float f; eiUint32 i; } vx = { x };
	float qpprox;
	vx.i &= 0x7FFFFFFF;

	qpprox = 1.0f - twooverpi * vx.f;

	return qpprox + p * qpprox * (1.0f - qpprox * qpprox);
}

inline float fastcosfull(float x)
{
	const float halfpi = 1.5707963267948966f;
	return fastsinfull (x + halfpi);
}

inline float fastercosfull(float x)
{
	const float halfpi = 1.5707963267948966f;
	return fastersinfull (x + halfpi);
}

inline float fasttan(float x)
{
	const float halfpi = 1.5707963267948966f;
	return fastsin (x) / fastsin (x + halfpi);
}

inline float fastertan(float x)
{
	return fastersin (x) / fastercos (x);
}

inline float fasttanfull(float x)
{
	const float twopi = 6.2831853071795865f;
	const float invtwopi = 0.15915494309189534f;

	int k = (int)(x * invtwopi);
	float half = (x < 0) ? -0.5f : 0.5f;
	float xnew = x - (half + k) * twopi;

	return fastsin (xnew) / fastcos (xnew);
}

inline float fastertanfull(float x)
{
	const float twopi = 6.2831853071795865f;
	const float invtwopi = 0.15915494309189534f;

	int k = (int)(x * invtwopi);
	float half = (x < 0) ? -0.5f : 0.5f;
	float xnew = x - (half + k) * twopi;

	return fastersin (xnew) / fastercos (xnew);
}

/** Polynomial fast arcsine approximation, error bounds of ~1.5% 
 * until within 1% of domain bounds */
inline float fastasin(float x)
{
	static const float scale_factor = 0.391f;
	float x5 = x * x;
	x5 *= x5;
	x5 *= x;
	return x + scale_factor * x5;
}

/** Polynomial fast arccos approximation with good error bounds 
 * until within ~99% of domain edge */
inline float fastacos(float x)
{
	return ((eiScalar)EI_PI * 0.5f) - fastasin(x);
}

/** A simple cubic approximation for arccos.
 * the maximum error is about 0.18 radians. */
inline float fasteracos(float x)
{
	return (-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f;
}

inline eiScalar radians(eiScalar x)
{
	return (x * ((eiScalar)EI_PI / 180.0f));
}

inline eiScalar degrees(eiScalar x)
{
	return (x * (180.0f / (eiScalar)EI_PI));
}

inline eiInt ltruncf(eiScalar a)
{
	return (eiInt)a;
}

inline eiInt lfloorf(eiScalar a)
{
	return (eiInt)a - (a < 0.0f);
}

inline eiInt lceilf(eiScalar a)
{
	const eiInt d = (eiInt)a;
	return d + (a > 0.0f && a != (eiScalar)d);
}

inline eiInt ei_lroundf(eiScalar a)
{
	return lfloorf(a + 0.5f);
}

inline eiScalar lfracf(eiScalar a)
{
	return a - (eiScalar)lfloorf(a);
}

inline eiScalar fmod1(eiScalar a)
{
	return a - (eiScalar)((eiInt)a);
}

inline eiScalar sign(eiScalar a)
{
	if (a == 0.0f) {
		return 0.0f;
	} else if (a > 0.0f) {
		return 1.0f;
	} else {
		return -1.0f;
	}
}

inline eiBool almost_zero(eiScalar a, eiScalar eps)
{
	return (absf(a) < eps);
}

inline eiBool almost_equal(eiScalar a, eiScalar b, eiScalar eps)
{
	return almost_zero(a - b, eps);
}

#ifndef min

template <typename T>
inline T min(T a, T b)
{
	return ((a < b) ? a : b);
}

#endif

#ifndef max

template <typename T>
inline T max(T a, T b)
{
	return ((a > b) ? a : b);
}

#endif

template <typename T>
inline T clamp(T x, T lo, T hi)
{
	return (x < lo) ? lo : ((x > hi) ? hi : x);
}

template <typename T>
inline T lerp(T a, T b, eiScalar t)
{
   return ((1.0f - t) * a) + (b * t);
}

inline eiScalar step(eiScalar lo, eiScalar value)
{
	if (value < lo) {
		return 0.0f;
	} else {
		return 1.0f;
	}
}

inline eiScalar safe_acos(eiScalar x)
{
	const eiScalar T = clamp(x, -1.0f, 1.0f);
	return acosf(T);
}

#ifndef EI_NO_SMOOTHSTEP

inline eiScalar smoothstep(eiScalar lo, eiScalar hi, eiScalar value)
{
	if (value < lo) {
		return 0.0f;
	} else if (value >= hi) {
		return 1.0f;
	} else {
		eiScalar t = (value - lo) / (hi - lo);
		return lerp(0.0f, 1.0f, t * t * (3.0f - 2.0f * t));
	}
}

#endif

inline void find_sorted_no_minmax(
	eiScalar & no_min, eiScalar & no_max, 
	eiScalar min1, eiScalar max1, 
	eiScalar min2, eiScalar max2)
{
	if (max1 < min2) {
		no_min = max1;
		no_max = min2;
	} else if (max2 < min1) {
		no_min = max2;
		no_max = min1;
	} else {
		if (min1 < min2) {
			no_min = min2;
		} else {
			no_min = min1;
		}
		if (max1 < max2) {
			no_max = max1;
		} else {
			no_max = max2;
		}
	}
}

inline eiBool is_hit_1d(
	eiScalar amax, eiScalar amin, 
	eiScalar bmax, eiScalar bmin, 
	eiScalar & hit_max, eiScalar & hit_min)
{
	if (amin > bmax || bmin > amax) {
		return EI_FALSE;
	} else {
		find_sorted_no_minmax(hit_min, hit_max, amin, amax, bmin, bmax);
		return EI_TRUE;
	}
}

/** Compute the system luminance.
 * we use Y601 for a more perceptually based luminance.
 */
inline eiScalar ei_luminance(const eiScalar r, const eiScalar g, const eiScalar b)
{
	return (0.30f * r + 0.59f * g + 0.11f * b);
}

inline eiGeoScalar ei_luminance(const eiGeoScalar r, const eiGeoScalar g, const eiGeoScalar b)
{
	return (0.30 * r + 0.59 * g + 0.11 * b);
}

#endif
