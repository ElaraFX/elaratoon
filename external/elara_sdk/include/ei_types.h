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

#ifndef EI_TYPES_H
#define EI_TYPES_H

/** Definitions of basic types.
 * \file ei_types.h
 */

#include <ei_base.h>
#include <stddef.h>

#define EI_FALSE				0
#define EI_TRUE					1

typedef signed char				eiInt8;
typedef unsigned char			eiUint8;

typedef signed short			eiInt16;
typedef unsigned short			eiUint16;

typedef signed int				eiInt32;
typedef unsigned int			eiUint32;

typedef signed long long		eiInt64;
typedef unsigned long long		eiUint64;

typedef float					eiScalar;
typedef double					eiGeoScalar;

typedef eiUint8					eiByte;
typedef eiInt16					eiShort;
typedef eiUint16				eiUshort;
typedef eiInt32					eiInt;
typedef eiUint32				eiUint;
typedef eiInt64					eiLong;
typedef eiUint64				eiUlong;

#ifdef EI_ARCH_X86
typedef eiInt32					eiIntptr;
typedef eiUint32				eiUintptr;
#else
typedef eiInt64					eiIntptr;
typedef eiUint64				eiUintptr;
#endif

typedef eiUintptr				eiSizet;

typedef eiUintptr				eiTag;
typedef eiUint					eiIndex;
typedef eiInt					eiBool;

typedef eiInt					eiThreadID;

typedef void *					eiInterface;

#define EI_SCALAR_EPS			(1.0e-5f)
#define EI_GEOSCALAR_EPS		(1e-9)
#define EI_NORM_EPS				(1.0e-30f)				/* must be small enough to avoid 
														   geometric artifacts for normalize */
#define EI_COS_DEG_89_9			0.00174532836589830886f	/* cosd(89.9) */
#define EI_COS_DEG_84			0.10452846326765347139f	/* cosd(84) now only for translucent */
#define EI_BIG_SCALAR			(1e+30f)
#define EI_MAX_SCALAR			(3.402823466e+38f)
#define EI_MAX_GEOSCALAR		(1.7976931348623158e+308)
#define EI_MIN_SHORT			(-32767 - 1)
#define EI_MAX_SHORT			(32767)
#define EI_MIN_INT				(-2147483647 - 1)
#define EI_MAX_INT				(2147483647)
#define EI_MIN_LONG				(-9223372036854775807i64 - 1)
#define EI_MAX_LONG				(9223372036854775807i64)
#define EI_NULL_INDEX			(0xFFFFFFFF)
#define EI_MAX_INDEX			(0xFFFFFFFE)
#define EI_NULL_TAG				(eiTag)(~((eiUintptr)0))
#define EI_MAX_SIZET			(eiSizet)(~((eiSizet)0))
#define EI_INVALID_HOSTID		-1
#define EI_INVALID_THREADID		-1

#define EI_E			2.71828182845904523536 /* e */
#define EI_LOG2E		1.44269504088896340736 /* log2(e) */
#define EI_LOG10E		0.434294481903251827651 /* log10(e) */
#define EI_LN2			0.693147180559945309417 /* ln(2) */
#define EI_LN10			2.30258509299404568402 /* ln(10) */
#define EI_PI			3.14159265358979323846 /* pi */
#define EI_PI_2			1.57079632679489661923 /* pi/2 */
#define EI_PI_4			0.785398163397448309616 /* pi/4 */
#define EI_1_PI			0.318309886183790671538 /* 1/pi */
#define EI_2_PI			0.636619772367581343076 /* 2/pi */
#define EI_2_SQRTPI		1.12837916709551257390 /* 2/sqrt(pi) */
#define EI_SQRT2		1.41421356237309504880 /* sqrt(2) */
#define EI_1_SQRT2		0.707106781186547524401 /* 1/sqrt(2) */

/** Token is an unique string in the system.
 */
typedef struct eiToken {
	const char		*str;	/**< the string pointer in host local address */
	eiTag			tag;	/**< the allocated string in database */
} eiToken;

static const eiToken g_NullToken = { NULL, EI_NULL_TAG };

#endif
