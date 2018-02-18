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

#ifndef EI_FIXED_POOL_H
#define EI_FIXED_POOL_H

/** Memory pool for allocating fixed-sized blocks.
 * \file ei_fixed_pool.h
 * \author Elvic Liang
 */

#include <ei_core.h>

/** Declaration of fixed memory pool.
 */
typedef struct ei_fixed_pool ei_fixed_pool;

/** Declaration of memory chunk node.
 */
typedef struct ei_chunk_node ei_chunk_node;

/** Fixed memory pool object.
 */
struct ei_fixed_pool {
	ei_chunk_node	*freeItems;			/**< linked list of free items */
	ei_chunk_node	*allocatedBanks;	/**< linked list of allocated banks */
	eiInt			dataSize;			/**< data size of each item */
	eiInt			itemsPerBank;		/**< the number of items per bank */
	eiUint			allocations;		/**< the number of allocations */
};

/** Initialize a fixed pool.
 */
EI_API void ei_fixed_pool_init(ei_fixed_pool *pool, const eiInt dataSize, const eiInt itemsPerBank);

/** Cleanup a fixed pool.
 */
EI_API void ei_fixed_pool_clear(ei_fixed_pool *pool);

/** Allocate a chunk of memory from the fixed pool.
 */
EI_API void *ei_fixed_pool_allocate(ei_fixed_pool *pool);

/** Free a chunk of memory to the fixed pool.
 */
EI_API void ei_fixed_pool_free(ei_fixed_pool *pool, void *item);

#endif
