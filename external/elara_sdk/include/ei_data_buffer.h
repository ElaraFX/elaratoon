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

#ifndef EI_DATA_BUFFER_H
#define EI_DATA_BUFFER_H

/** Generic data buffer stored in database, and can be 
 * referenced by tag.
 * \file ei_data_buffer.h
 * \author Elvic Liang
 */

#include <ei_api.h>

/** Create a buffer in database, returns its tag.
 */
EI_API eiTag ei_create_data_buffer(const eiInt type);

/** Delete the buffer from database.
 */
EI_API void ei_delete_data_buffer(const eiTag tag);

/** Allocate the buffer.
 */
EI_API void ei_data_buffer_allocate(
	const eiTag tag, 
	const eiInt w, 
	const eiInt h);

/** Get the size of buffer element in bytes.
 */
EI_API eiInt ei_data_buffer_get_item_size(const eiTag tag);
/** Get the data type of buffer element.
 */
EI_API eiInt ei_data_buffer_get_type(const eiTag tag);
/** Get the number of elements in this buffer.
 */
EI_API eiInt ei_data_buffer_get_size(const eiTag tag);
/** Get the width.
 */
EI_API eiInt ei_data_buffer_get_width(const eiTag tag);
/** Get the height.
 */
EI_API eiInt ei_data_buffer_get_height(const eiTag tag);

/** Get buffer element by coordinates.
 */
EI_API void ei_data_buffer_get(
	const eiTag tag, 
	eiInt x, 
	eiInt y, 
	void *val);

/** Get buffer element by coordinates in tiled mode.
 */
EI_API void ei_data_buffer_get_tiled(
	const eiTag tag, 
	eiInt x, 
	eiInt y, 
	void *val);

/** Set buffer element by coordinates.
 */
EI_API void ei_data_buffer_set(
	const eiTag tag, 
	eiInt x, 
	eiInt y, 
	const void *val);

/** Set buffer element by coordinates in tiled mode.
 */
EI_API void ei_data_buffer_set_clamped(
	const eiTag tag, 
	eiInt x, 
	eiInt y, 
	const void *val);

/** Zero the buffer memory.
 */
EI_API void ei_data_buffer_zero_memory(const eiTag tag);

/** Fill the buffer with an element.
 */
EI_API void ei_data_buffer_fill(
	const eiTag tag, 
	const void *val);

/** Clear the buffer.
 */
EI_API void ei_data_buffer_clear(const eiTag tag);

#endif
