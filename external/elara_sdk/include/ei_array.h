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

#ifndef EI_ARRAY_H
#define EI_ARRAY_H

/** Dynamic array container.
 * \file ei_array.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_assert.h>

/** Declaration of dynamic array container.
 */
typedef struct ei_array ei_array;

/** Array book-keeping object.
 */
struct ei_array {
	eiByte		*data;		/**< data block pointer */
	eiIntptr	item_size;	/**< size of a data item in bytes */
	eiIntptr	size;		/**< number of items in this array */
	eiIntptr	cap;		/**< capacity of this array */
};

/** Initialize an array.
 */
EI_API void ei_array_init(ei_array *arr, const eiIntptr item_size);

/** Clear the data in an array.
 */
EI_API void ei_array_clear(ei_array *arr);

/** Get a data item by index.
 */
inline void *ei_array_get(ei_array *arr, const eiIntptr index)
{
	EI_DBG_ASSERT(arr != NULL);

	return (arr->data + arr->item_size * index);
}

/** Return true when the array is empty.
 */
inline eiBool ei_array_empty(ei_array *arr)
{
	EI_DBG_ASSERT(arr != NULL);

	return (arr->size == 0);
}

/** Get the current size of the array.
 */
inline eiIntptr ei_array_size(ei_array *arr)
{
	EI_DBG_ASSERT(arr != NULL);

	return arr->size;
}

/** Get the capacity of the array, returns the C++ STL compatible value.
 */
EI_API eiIntptr ei_array_capacity(ei_array *arr);

/** Push a data item at the back.
 */
EI_API void ei_array_push_back(ei_array *arr, const void *item);

/** Erase a data item by index.
 */
EI_API void ei_array_erase(ei_array *arr, const eiIntptr index);

/** Copy from an existing array.
 */
EI_API void ei_array_copy(ei_array *dest, ei_array *src);

/** Reserve the size of the array for at least n elements.
 * This function is NOT C++ STL compatible, the n is the size to 
 * increase, not the resulting capacity.
 */
EI_API void ei_array_reserve(ei_array *arr, const eiIntptr n);

/** Resize the array to n elements. This function is C++ STL 
 * compatible, the n is the resulting size of this container.
 */
EI_API void ei_array_resize(ei_array *arr, const eiIntptr n);

/** Get the front data item.
 */
EI_API void *ei_array_front(ei_array *arr);

/** Get the back data item.
 */
EI_API void *ei_array_back(ei_array *arr);

/** Get the data buffer of the array.
 * this function is identical to ei_array_front, 
 * being here just for semantic sake.
 */
inline void *ei_array_data(ei_array *arr)
{
	EI_DBG_ASSERT(arr != NULL);

	return arr->data;
}

#endif
