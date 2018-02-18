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

#ifndef EI_POOL_H
#define EI_POOL_H

/** Generic memory pool for fast allocations of 
 * varying-sized small objects.
 * \file ei_pool.h
 * \author Elvic Liang
 */

#include <ei_core.h>

/** Declaration of generic memory pool.
 */
typedef struct ei_pool ei_pool;

/** Declaration of memory chunk header.
 */
typedef union ei_chunk_header ei_chunk_header;

/** Generic memory pool object.
 */
struct ei_pool {
	ei_chunk_header		**memoryChunks;
	eiByte				*memoryPage;
	eiSizet				memoryAvailable;
	eiSizet				memoryUsage;
	ei_chunk_header		*memoryAllPages;
	/* statistics */
	eiUint				page_allocations;
	eiUint				huge_allocations;
};

/** Initialize a pool.
 */
EI_API void ei_pool_init(ei_pool *pool);

/** Cleanup a pool.
 */
EI_API void ei_pool_clear(ei_pool *pool);

/** Allocate a chunk of memory from the pool.
 */
EI_API void *ei_pool_allocate(ei_pool *pool, eiSizet size);

/** Free a chunk of memory to the pool.
 */
EI_API void ei_pool_free(ei_pool *pool, void *ptr);

#endif
