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

#ifndef EI_MATRIX2_H
#define EI_MATRIX2_H

#include <ei_util.h>
#include <string.h>

struct eiMatrix2 {
	union {
		struct {
			eiScalar	m1, m2;
			eiScalar	m3, m4;
		};
		eiScalar		comp[4];
		eiScalar		m[2][2];
	};
};

inline void setm2(eiMatrix2 *r, 
	const eiScalar _m1, const eiScalar _m2, 
	const eiScalar _m3, const eiScalar _m4)
{
	r->m1 = _m1; r->m2 = _m2;
	r->m3 = _m3; r->m4 = _m4;
}

inline void movm2(eiMatrix2 *a, const eiMatrix2 *b)
{
	memcpy(a, b, sizeof(eiMatrix2));
}

#endif
