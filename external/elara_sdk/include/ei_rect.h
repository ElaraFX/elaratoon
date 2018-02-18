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

#ifndef EI_RECT_H
#define EI_RECT_H

#include <ei_util.h>
#include <string.h>

/** Integer rectangle
 */
struct eiRect4i {

	eiInt		left;
	eiInt		right;
	eiInt		top;
	eiInt		bottom;

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	inline void AddWithWeight(eiRect4i const & src, float weight)
	{
		left = (eiInt)((float)left + weight * (float)src.left);
		right = (eiInt)((float)right + weight * (float)src.right);
		top = (eiInt)((float)top + weight * (float)src.top);
		bottom = (eiInt)((float)bottom + weight * (float)src.bottom);
	}
};

inline void ei_rect4i_init(eiRect4i *r)
{
	r->left = 0;
	r->right = 0;
	r->top = 0;
	r->bottom = 0;
}

inline void ei_rect4i_set(
	eiRect4i *r, 
	const eiInt rleft, const eiInt rright,
	const eiInt rtop, const eiInt rbottom)
{
	r->left = rleft;
	r->right = rright;
	r->top = rtop;
	r->bottom = rbottom;
}

inline void ei_rect4i_copy(eiRect4i *a, const eiRect4i *b)
{
	memcpy(a, b, sizeof(eiRect4i));
}

/** Scalar rectangle
 */
struct eiRect {

	eiScalar	left;
	eiScalar	right;
	eiScalar	top;
	eiScalar	bottom;

	// OpenSubdiv interop
	inline void Clear(void * = 0)
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	inline void AddWithWeight(eiRect const & src, float weight)
	{
		left += weight * src.left;
		right += weight * src.right;
		top += weight * src.top;
		bottom += weight * src.bottom;
	}
};

inline void ei_rect_init(eiRect *r)
{
	r->left = 0.0f;
	r->right = 0.0f;
	r->top = 0.0f;
	r->bottom = 0.0f;
}

inline void ei_rect_set(
	eiRect *r, 
	const eiScalar rleft, const eiScalar rright,
	const eiScalar rtop, const eiScalar rbottom)
{
	r->left = rleft;
	r->right = rright;
	r->top = rtop;
	r->bottom = rbottom;
}

inline void ei_rect_copy(eiRect *a, const eiRect *b)
{
	memcpy(a, b, sizeof(eiRect));
}

#endif
