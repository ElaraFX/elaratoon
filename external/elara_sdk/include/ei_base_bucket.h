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

#ifndef EI_BASE_BUCKET_H
#define EI_BASE_BUCKET_H

#include <ei_state.h>
#include <ei_raytracer.h>
#include <ei_nodesys.h>
#include <ei_frame_buffer.h>
#include <ei_data_table.h>
#include <ei_random.h>
#include <ei_rect.h>

#pragma pack(push, 1)

/** Job of an image bucket.
 */
struct eiBucketJob {
	eiInt			pos_i;
	eiInt			pos_j;
	eiRect4i		rect;
	eiTag			infoFrameBuffer;
	eiTag			colorFrameBuffer;
	eiTag			opacityFrameBuffer;
	eiTag			frameBuffers;			/**< the data array of frame buffer tags */
	eiScalar		point_spacing;
	eiInt			pass_id;
	eiRect4i		s_win;					/**< sampling window */
};

#pragma pack(pop)

#endif
