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

#ifndef EI_DATA_TABLE_H
#define EI_DATA_TABLE_H

/** Generic data table which is stored in database, and 
 * can be referenced by tag.
 * It provides native support for motion keys.
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_dataflowx.h>

/** Internal header of data table.
 */
#pragma pack(push, 1)
typedef struct eiDataTable {
	eiByte		type;		/**< the type of each item */
	eiByte		nkeys;		/**< the number of motion keys */
	eiUshort	item_size;	/**< the size of each data item */
	eiInt		size;		/**< the number of items in this data table */
	eiInt		cap;		/**< the capacity of this data table */
	eiInt		nelements;	/**< the number of elements (size / nkeys), align at 16 byte boundary for SSE */
} eiDataTable;
#pragma pack(pop)

/** The iterator for speeding up data table accesses.
 */
typedef struct eiDataTableIterator {
	eiByte			*data;
	eiTag			tag;
	eiUint			item_size;
	eiUint			nkeys;
	eiUint			nkeys_1;
	eiInt			item_type;
	eiInt			item_count;
	eiUint			padding;
} eiDataTableIterator;

/** Initialize a data table in database, and returns its tag.
 */
EI_API eiTag ei_create_data_table(const eiInt type, const eiUint nkeys);

/** Delete a data table by its tag.
 */
EI_API void ei_delete_data_table(const eiTag tag);

/** Clone a data table.
 */
EI_API eiTag ei_clone_data_table(const eiTag src_tag);

/** Clear all data in the data table.
 */
EI_API void ei_data_table_clear(const eiTag tag);

/** Reserve to size * nkeys.
 */
EI_API void ei_data_table_reserve(const eiTag tag, const eiUint size);

/** Resize to size * nkeys.
 */
EI_API void ei_data_table_resize(const eiTag tag, const eiInt size);

/** Begin accessing the data table.
 */
EI_API void ei_data_table_begin(const eiTag tag, eiDataTableIterator * const iter);

/** Begin reading a data item by index.
 */
inline void *ei_data_table_read(eiDataTableIterator * const iter, const eiInt index, const eiUint key)
{
	const eiIndex item_index = index * iter->nkeys + MIN(iter->nkeys_1, key);

	return (iter->data + iter->item_size * item_index);
}

/** Faster reading function for the first motion key.
 */
inline void *ei_data_table_get(eiDataTableIterator * const iter, const eiInt index)
{
	return (iter->data + iter->item_size * (index * iter->nkeys));
}

/** Begin reading/writing a data item by index.
 */
inline void *ei_data_table_write(eiDataTableIterator * const iter, const eiInt index, const eiUint key)
{
	const eiIndex item_index = index * iter->nkeys + MIN(iter->nkeys_1, key);

	void *ptr = (iter->data + iter->item_size * item_index);

	ei_db_dirt(iter->tag);

	return ptr;
}

/** Dirt the entire data table.
 */
inline void ei_data_table_dirt(eiDataTableIterator * const iter)
{
	ei_db_dirt(iter->tag);
}

/** End accessing the data table.
 */
EI_API void ei_data_table_end(eiDataTableIterator * const iter);

/** Return true when the data table is empty.
 */
EI_API eiBool ei_data_table_empty(const eiTag tag);

/** Get the type of the data table.
 */
EI_API eiInt ei_data_table_type(const eiTag tag);

/** Get the current size of the data table.
 */
EI_API eiInt ei_data_table_size(const eiTag tag);

/** Get the number of motion keys in the data table.
 */
EI_API eiUint ei_data_table_num_keys(const eiTag tag);

/** Push a data item at the back.
 */
EI_API void ei_data_table_push_back(const eiTag tag, const void *item);

/** Reset the data table iterator to an empty one.
 */
EI_API void ei_data_table_reset_iterator(eiDataTableIterator *iter);

/** Compute i0 * t0 + i1 * t1.
 */
EI_API void ei_data_table_lerp(
	eiDataTableIterator * const iter, 
	void *result, 
	const eiInt i0, 
	const eiInt i1, 
	const eiUint key, 
	eiScalar t0, 
	eiScalar t1);

/** Compute i0 * bary->x + i1 * bary->y + i2 * bary->z.
 */
EI_API void ei_data_table_berp(
	eiDataTableIterator * const iter, 
	void *result, 
	const eiInt i0, 
	const eiInt i1, 
	const eiInt i2, 
	const eiUint key, 
	const eiVector *bary);

/** Get the total memory in bytes that this data table consumes.
 */
EI_API eiInt ei_data_table_total_mem(const eiTag tag);

/** Accessor for data table.
 */
template <typename T>
class eiDataTableAccessor
{
public:
	inline eiDataTableAccessor(const eiTag tag)
	{
		ei_data_table_begin(tag, &m_iter);

		EI_DBG_ASSERT(tag == EI_NULL_TAG || sizeof(T) == m_iter.item_size);
	}

	inline ~eiDataTableAccessor()
	{
		ei_data_table_end(&m_iter);
	}

	inline void release()
	{
		const eiTag tag = m_iter.tag;

		ei_data_table_end(&m_iter);

		if (tag != EI_NULL_TAG)
		{
			ei_delete_data_table(tag);
		}
	}

	inline void clear()
	{
		const eiTag tag = m_iter.tag;

		ei_data_table_end(&m_iter);

		if (tag != EI_NULL_TAG)
		{
			ei_data_table_clear(tag);
		}
	}

	inline eiInt size() const
	{
		return m_iter.item_count;
	}

	inline eiUint num_keys() const
	{
		return m_iter.nkeys;
	}

	inline T & get(const eiInt index, const eiUint key)
	{
		return *(((T *)ei_data_table_read(&m_iter, index, key)));
	}

	inline T & get(const eiInt index)
	{
		return *(((T *)ei_data_table_get(&m_iter, index)));
	}

	inline T & set(const eiInt index, const eiUint key)
	{
		return *(((T *)ei_data_table_write(&m_iter, index, key)));
	}

	inline void dirt()
	{
		ei_data_table_dirt(&m_iter);
	}

	inline void lerp(
		void *result, 
		const eiInt i0, 
		const eiInt i1, 
		const eiUint key, 
		eiScalar t0, 
		eiScalar t1)
	{
		ei_data_table_lerp(
			&m_iter, 
			result, 
			i0, 
			i1, 
			key, 
			t0, 
			t1);
	}

	inline void berp(
		void *result, 
		const eiInt i0, 
		const eiInt i1, 
		const eiInt i2, 
		const eiUint key, 
		const eiVector & bary)
	{
		ei_data_table_berp(
			&m_iter, 
			result, 
			i0, 
			i1, 
			i2, 
			key, 
			&bary);
	}

private:
	eiDataTableIterator		m_iter;
};

#endif
