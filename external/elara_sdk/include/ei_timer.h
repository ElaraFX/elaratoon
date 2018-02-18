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

#ifndef EI_TIMER_H
#define EI_TIMER_H

/** Utilities for timing and profiling.
 * \file ei_timer.h
 */

#include <ei_core.h>

/** Timer for multiple time segments.
 */
struct eiTimer
{
	eiInt		start_time;	/**< the starting time record */
	eiInt		duration;	/**< the accumulated duration in milliseconds */
};

/** Restart a timer.
 */
EI_API void ei_timer_reset(eiTimer *timer);

/** Start a timer.
 */
EI_API void ei_timer_start(eiTimer *timer);

/** Stop a timer, accumulate passed duration.
 */
EI_API void ei_timer_stop(eiTimer *timer);

/** Format the duration of a timer into hours, minutes, seconds.
 */
EI_API void ei_timer_format(
	eiTimer *timer, 
	eiInt *hours, 
	eiInt *minutes, 
	eiScalar *seconds);

#endif
