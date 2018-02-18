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

#ifndef EI_FRAME_BUFFER_H
#define EI_FRAME_BUFFER_H

/** Generic frame buffer.
 * \file ei_frame_buffer.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_data_buffer.h>
#include <ei_color.h>
#include <ei_vector2.h>
#include <ei_vector4.h>
#include <ei_buffer.h>
#include <ei.h>
#include <ei_rect.h>

#pragma pack(push, 1)

/** The per-pixel information */
typedef struct eiPixelInfo {
	eiUint		num_samples;		/**< the number of sub-pixel samples within this pixel */
	eiScalar	weight;				/**< the accumulated pixel weight */
	eiScalar	unfilter_weight;	/**< the weight for frame buffers that have filter off */
	eiColor		mean_color;			/**< mean value of all channels (RGB) */
	eiScalar	mean_differential;	/**< mean differential */
	eiUint		samples_coherence;	/**< samples coherence number of mean differential */
} eiPixelInfo;

/** The interface for accessing a tile on 
 * a frame buffer.
 */
typedef struct eiFrameBufferCache {
	const char			*m_unique_name;
	eiTag				m_fb;
	eiInt				m_width;
	eiInt				m_height;
	eiInt				m_border;
	eiBool				m_filter;
	eiScalar			m_point_spacing;
	eiScalar			m_inv_point_spacing;
	eiInt				m_pass_id;
	eiInt				m_type;					/**< data type of frame buffer elements, copied 
												     from frame buffer for fast access */
	eiInt				m_data_size;
	eiUint				m_data_offset;			/**< data offset in bytes in sample info block, 
												     copied from frame buffer for fast access */
	eiInt				m_width1;
	eiInt				m_height1;
	eiInt				m_i;
	eiInt				m_j;
	eiInt				m_tile_left;
	eiInt				m_tile_top;
	eiScalar			m_display_gamma;
	const eiToneOp		*m_toneop;
	eiByte				*m_ptr;
	eiTag				m_tile_tag;
	ei_buffer_copy_item			m_copy_item;
	struct eiFrameBufferCache	*m_weight_cache;
	eiRect4i			m_rect;
} eiFrameBufferCache;

#pragma pack(pop)

inline void divide_weight(
	const eiInt type, 
	void *data, 
	const eiScalar weight, 
	const eiScalar display_gamma, 
	const eiToneOp *toneop)
{
	eiScalar inv_weight = 0.0f;

	if (weight != 0.0f)
	{
		inv_weight = rcpf(weight);
	}

	switch (type)
	{
	case EI_TYPE_SCALAR:
		*((eiScalar *)data) *= inv_weight;
		/* currently tone mapping does not support mono image */
		ei_gamma(*((eiScalar *)data), display_gamma);
		break;
	case EI_TYPE_COLOR:
		*((eiColor *)data) *= inv_weight;
		/* do tone mapping before gamma correction */
		if (toneop != NULL)
		{
			ei_toneop_apply(toneop, *((eiColor *)data));
		}
		ei_gamma(*((eiColor *)data), display_gamma);
		break;
	default:
		/* error */
		break;
	}
}

EI_API void ei_framebuffer_cache_init(
	eiFrameBufferCache *cache, 
	const eiTag fb, 
	const eiInt i, 
	const eiInt j, 
	const eiScalar point_spacing, 
	const eiInt pass_id, 
	eiFrameBufferCache *weight_cache);
EI_API void ei_framebuffer_cache_exit(eiFrameBufferCache *cache);

/** Flush changes of a frame buffer cache.
 */
EI_API void ei_framebuffer_cache_flush(eiFrameBufferCache *cache);

inline const char *ei_framebuffer_cache_get_unique_name(eiFrameBufferCache *cache)
{
	return cache->m_unique_name;
}

/** Get the tile width not including border size.
 */
inline eiInt ei_framebuffer_cache_get_width(eiFrameBufferCache *cache)
{
	return cache->m_width - 2 * cache->m_border;
}

/** Get the tile height not including border size.
 */
inline eiInt ei_framebuffer_cache_get_height(eiFrameBufferCache *cache)
{
	return cache->m_height - 2 * cache->m_border;
}

inline eiInt ei_framebuffer_cache_get_border(eiFrameBufferCache *cache)
{
	return cache->m_border;
}

inline eiInt ei_framebuffer_cache_get_type(eiFrameBufferCache *cache)
{
	return cache->m_type;
}

inline eiInt ei_framebuffer_cache_get_data_size(eiFrameBufferCache *cache)
{
	return cache->m_data_size;
}

inline eiUint ei_framebuffer_cache_get_data_offset(eiFrameBufferCache *cache)
{
	return cache->m_data_offset;
}

inline void ei_framebuffer_cache_get(
	eiFrameBufferCache *cache, 
	eiInt x, 
	eiInt y, 
	void *data)
{
	/* take border into account */
	x += cache->m_border;
	y += cache->m_border;

	x = clamp(x, 0, cache->m_width1);
	y = clamp(y, 0, cache->m_height1);
	cache->m_copy_item(data, 
		cache->m_ptr + (x + y * cache->m_width) * cache->m_data_size, 
		cache->m_data_size);
}

EI_API void ei_framebuffer_cache_round_raster(
	eiFrameBufferCache *cache, 
	eiInt & x, 
	eiInt & y);

/** This function should be used in client applications 
 * to get final pixel color rather than the ei_framebuffer_cache_get 
 * function, which only gets the raw data.
 */
EI_API void ei_framebuffer_cache_get_final(
	eiFrameBufferCache *cache, 
	eiInt x, 
	eiInt y, 
	void *data);

inline void ei_framebuffer_cache_set(
	eiFrameBufferCache *cache, 
	eiInt x, 
	eiInt y, 
	const void *data)
{
	/* take border into account */
	x += cache->m_border;
	y += cache->m_border;

	if (x >= 0 && x < cache->m_width && y >= 0 && y < cache->m_height)
	{
		cache->m_copy_item(cache->m_ptr + (x + y * cache->m_width) * cache->m_data_size, 
			data, 
			cache->m_data_size);
	}
}

inline eiScalar filter_NaN(const eiScalar x, eiUint & num_NaNs)
{
	if (!(x > -10000000.0f && x < 10000000.0f))
	{
		++ num_NaNs;
		return 0.0f; /* flush NaN to zero */
	}

	return x;
}

/** Multiply the data with weight, then accumulate the 
 * result into frame buffer.
 * return the number of NaNs found in adds.
 */
inline eiUint ei_framebuffer_cache_add(
	eiFrameBufferCache *cache, 
	eiInt x, 
	eiInt y, 
	const void *data, 
	const eiScalar weight)
{
	eiUint num_NaNs = 0;

	/* take border into account */
	x += cache->m_border;
	y += cache->m_border;

	if (x >= 0 && x < cache->m_width && y >= 0 && y < cache->m_height)
	{
		eiByte *dest = cache->m_ptr + (x + y * cache->m_width) * cache->m_data_size;

		switch (cache->m_type)
		{
		case EI_TYPE_SCALAR:
			*((eiScalar *)dest) += filter_NaN((*((eiScalar *)data)) * weight, num_NaNs);
			break;
		case EI_TYPE_COLOR:
			/* this is a performance bottleneck, expand it to make it faster */
			((eiColor *)dest)->r += filter_NaN(((const eiColor *)data)->r * weight, num_NaNs);
			((eiColor *)dest)->g += filter_NaN(((const eiColor *)data)->g * weight, num_NaNs);
			((eiColor *)dest)->b += filter_NaN(((const eiColor *)data)->b * weight, num_NaNs);
			break;
		case EI_TYPE_PIXEL_INFO:
			/* we should not change num_samples */
			((eiPixelInfo *)dest)->weight += filter_NaN(((eiPixelInfo *)data)->weight, num_NaNs); /* weight is always 1.0 for this case */
			((eiPixelInfo *)dest)->unfilter_weight += filter_NaN(((eiPixelInfo *)data)->unfilter_weight, num_NaNs); /* weight is always 1.0 for this case */
			break;
		default:
			/* error */
			break;
		}
	}

	return num_NaNs;
}

#pragma pack(push, 1)

/** A frame buffer stored in database.
 */
typedef struct eiFrameBuffer {
	eiInt			m_type;
	eiUint			m_data_offset;		/**< data offset in bytes in sample info block */
	eiInt			m_width;
	eiInt			m_height;
	eiInt			m_bucket_size;
	eiInt			m_border;
	eiNode			*m_cam_node;
	eiBool			m_filter;			/**< whether filtering is enabled for this frame buffer */
	eiInt			m_num_xbuckets;
	eiInt			m_num_ybuckets;
	eiInt			m_num_xbuckets1;
	eiInt			m_num_ybuckets1;
	eiScalar		m_display_gamma;
	const eiToneOp	*m_toneop;
	eiTag			m_buckets;			/**< a data buffer of tags, each tag points to 
										     a frame buffer tile */
} eiFrameBuffer;

#pragma pack(pop)

inline const char *ei_framebuffer_get_name(eiFrameBuffer *fb)
{
	return (const char *)(fb + 1);
}

/** Create a frame buffer in database.
 */
EI_API eiTag ei_create_framebuffer(
	const char *name, 
	const eiInt type, 
	const eiUint data_offset, 
	const eiInt w, 
	const eiInt h, 
	const eiInt bs, 
	const eiInt border, 
	eiNode *cam_node, 
	const eiBool filter, 
	const eiScalar display_gamma, 
	const eiToneOp *toneop);
/** Delete the frame buffer from database.
 */
EI_API void ei_delete_framebuffer(const eiTag tag);

/** Clear the frame buffer to all zero.
 */
EI_API void ei_clear_framebuffer(const eiTag tag);

/** Divide the first frame buffer by the second one.
 */
EI_API void ei_divide_framebuffer(
	const eiTag tag, 
	const eiTag infoTag);

/** Get buffer element by coordinates.
 */
EI_API void ei_framebuffer_get(
	eiFrameBuffer *fb, 
	eiInt x, 
	eiInt y, 
	void *val);

/** Get the number of actual pixels (including border pixels) in 
 * this frame buffer.
 */
EI_API eiUlong ei_framebuffer_get_actual_pixels(const eiTag tag);

#endif
