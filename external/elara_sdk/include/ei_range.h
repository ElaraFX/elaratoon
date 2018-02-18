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

#ifndef EI_RANGE_H
#define EI_RANGE_H

#include <ei_util.h>

struct eiRange {
	eiScalar	min;
	eiScalar	max;
};

inline void ei_range_init(eiRange *r)
{
	r->min = 0.0f;
	r->max = 0.0f;
}

inline void ei_range_set(eiRange *r, const eiScalar rmin, const eiScalar rmax)
{
	r->min = rmin;
	r->max = rmax;
}

inline void ei_range_copy(eiRange *a, const eiRange *b)
{
	memcpy(a, b, sizeof(eiRange));
}

inline eiBool ei_range_cover(const eiRange *r, const eiScalar x)
{
	return (x >= r->min && x <= r->max);
}

#endif
