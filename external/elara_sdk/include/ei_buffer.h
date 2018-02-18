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

#ifndef EI_BUFFER_H
#define EI_BUFFER_H

/** Generic 2D buffer container.
 * \file ei_buffer.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_color.h>
#include <ei_vector2.h>
#include <ei_vector4.h>

/** The callback to construct an item */
typedef void (*ei_buffer_init_item)(void *item);
/** The callback to destruct an item */
typedef void (*ei_buffer_exit_item)(void *item);
/** The callback to perform generic dst_item = src_item */
typedef void (*ei_buffer_copy_item)(void *dst_item, const void *src_item, const eiInt item_size);

/** This class represents a generic 2D array
 */
typedef struct eiBuffer {
	ei_buffer_init_item		m_init_item;
	ei_buffer_exit_item		m_exit_item;
	ei_buffer_copy_item		m_copy_item;

	eiByte		*m_data;
	eiInt		m_item_size;
	eiInt		m_size;
	eiInt		m_width;
	eiInt		m_height;
	eiInt		m_width1;		/**< m_width - 1 */
	eiInt		m_height1;		/**< m_height - 1 */
} eiBuffer;

/** Initialize a buffer.
 */
EI_API void ei_buffer_init(
	eiBuffer *buffer, 
	const eiInt item_size, 
	ei_buffer_init_item init_item, 
	ei_buffer_exit_item exit_item);

/** Allocate the buffer.
 */
EI_API void ei_buffer_allocate(
	eiBuffer *buffer, 
	const eiInt w, 
	const eiInt h);

/** Get size of each buffer element in bytes.
 */
inline eiInt ei_buffer_get_item_size(eiBuffer *buffer)
{
	return buffer->m_item_size;
}
/** Get the number of elements in this buffer.
 */
inline eiInt ei_buffer_get_size(eiBuffer *buffer)
{
	return buffer->m_size;
}
/** Get the width.
 */
inline eiInt ei_buffer_get_width(eiBuffer *buffer)
{
	return buffer->m_width;
}
/** Get the height.
 */
inline eiInt ei_buffer_get_height(eiBuffer *buffer)
{
	return buffer->m_height;
}

/** Get data pointer by coordinates.
 */
inline void *ei_buffer_getptr(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y)
{
	x = clamp(x, 0, buffer->m_width1);
	y = clamp(y, 0, buffer->m_height1);
	return (buffer->m_data + ((eiIntptr)x + (eiIntptr)y * (eiIntptr)buffer->m_width) * (eiIntptr)buffer->m_item_size);
}

/** Get data by coordinates.
 */
inline void ei_buffer_get(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y, 
	void *val)
{
	buffer->m_copy_item(val, ei_buffer_getptr(buffer, x, y), buffer->m_item_size);
}

/** Get data pointer by coordinates in tiled mode.
 */
inline void *ei_buffer_getptr_tiled(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y)
{
	if (x < 0) {
		x = buffer->m_width - ((-x) % buffer->m_width);
	} else if (x >= buffer->m_width) {
		x = x % buffer->m_width;
	}
	if (y < 0) {
		y = buffer->m_height - ((-y) % buffer->m_height);
	} else if (y >= buffer->m_height) {
		y = y % buffer->m_height;
	}
	return (buffer->m_data + ((eiIntptr)x + (eiIntptr)y * (eiIntptr)buffer->m_width) * (eiIntptr)buffer->m_item_size);
}

/** Get data by coordinates in tiled mode.
 */
inline void ei_buffer_get_tiled(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y, 
	void *val)
{
	buffer->m_copy_item(val, ei_buffer_getptr_tiled(buffer, x, y), buffer->m_item_size);
}

/** Set data by coordinates.
 */
inline void ei_buffer_set(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y, 
	const void *val)
{
	if (x >= 0 && x < buffer->m_width && y >= 0 && y < buffer->m_height)
	{
		buffer->m_copy_item(
			buffer->m_data + ((eiIntptr)x + (eiIntptr)y * (eiIntptr)buffer->m_width) * (eiIntptr)buffer->m_item_size, 
			val, 
			buffer->m_item_size);
	}
}

/** Set data by coordinates with clamping.
 */
inline void ei_buffer_set_clamped(
	eiBuffer *buffer, 
	eiInt x, 
	eiInt y, 
	const void *val)
{
	x = clamp(x, 0, buffer->m_width1);
	y = clamp(y, 0, buffer->m_height1);
	buffer->m_copy_item(
		buffer->m_data + ((eiIntptr)x + (eiIntptr)y * (eiIntptr)buffer->m_width) * (eiIntptr)buffer->m_item_size, 
		val, 
		buffer->m_item_size);
}

/** Zero the buffer memory.
 */
EI_API void ei_buffer_zero_memory(eiBuffer *buffer);

/** Fill the buffer with an element.
 */
EI_API void ei_buffer_fill(
	eiBuffer *buffer, 
	const void *val);

/** Clear the buffer.
 */
EI_API void ei_buffer_clear(eiBuffer *buffer);

#endif
