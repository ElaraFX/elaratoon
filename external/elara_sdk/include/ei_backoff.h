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

#ifndef EI_BACKOFF_H
#define EI_BACKOFF_H

#include <ei_core.h>
#include <ei_atomic.h>
#include <ei_assert.h>

/* Approximately the number of "pause" instructions 
   that take the same time as an context switch */
#define EI_LOOPS_BEFORE_YIELD		16

/** Exponential back-off structure.
 */
typedef struct eiBackoff {
	eiInt32			count;
} eiBackoff;

/** Reset back-off.
 */
inline void ei_backoff_reset(eiBackoff *backoff)
{
	backoff->count = 1;
}

/** Pause a while.
 */
inline void ei_backoff_pause(eiBackoff *backoff)
{
	if (backoff->count <= EI_LOOPS_BEFORE_YIELD)
	{
		ei_pause(backoff->count);
		backoff->count *= 2; /* pause twice long the next time */
	}
	else
	{
		ei_yield(); /* pause so long that we'd rather yield CPU to scheduler */
	}
}

#endif
