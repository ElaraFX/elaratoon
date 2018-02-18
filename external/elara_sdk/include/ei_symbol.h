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

#ifndef EI_SYMBOL_H
#define EI_SYMBOL_H

/** Utilities for handling symbols.
 * \file ei_symbol.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_btree.h>
#include <ei_array.h>
#include <ei_algorithm.h>
#include <ei_platform.h>

/** A token map for mapping unique string to a data. 
 * this class is not thread-safe, users must ensure this from outside.
 */
typedef struct eiTokenMapNode {
	const char		*name;		/**< the unique name string */
	void			*data;		/**< this can be a tag or a user pointer */
} eiTokenMapNode;

typedef struct eiTokenMap {
	ei_array		nodes;
	void			*null;
} eiTokenMap;

inline eiIntptr ei_token_map_node_compare(void *lhs, void *rhs)
{
	/* we can compare the string pointer directly, because 
	   we assume all strings have been converted to unique 
	   strings using string table. */
	return (((eiIntptr)((eiTokenMapNode *)lhs)->name) - ((eiIntptr)((eiTokenMapNode *)rhs)->name));
}

EI_DEFINE_HEAPSORT(eiTokenMapNode, ei_token_map_node_compare);
EI_DEFINE_BINSEARCH(eiTokenMapNode, ei_token_map_node_compare);

/** Initialize the token map.
 * \param null The null value, should be EI_NULL_TAG for tags and 
 * NULL for user pointers.
 */
EI_API void ei_token_map_init(eiTokenMap *map, void * const null);

/** Cleanup the token map.
 */
EI_API void ei_token_map_exit(eiTokenMap *map);

/** Insert a token into the map.
 * \param name The name of the token
 * \param data The data of the token, can be either a tag or a user pointer
 */
EI_API void ei_token_map_add(eiTokenMap *map, const char *name, void * const data);

/** Sort the map to be searchable.
 */
EI_API void ei_token_map_sort(eiTokenMap *map);

/** Lookup a token by its name and returns its data.
 */
inline void *ei_token_map_find(eiTokenMap *map, const char *name)
{
	eiTokenMapNode		key;
	eiIntptr			node_index;

	key.name = name;
	node_index = ei_binsearch_eiTokenMapNode((eiTokenMapNode *)ei_array_data(&map->nodes), ei_array_size(&map->nodes), &key);

	if (node_index == -1)
	{
		return map->null;
	}

	return ((eiTokenMapNode *)ei_array_get(&map->nodes, node_index))->data;
}

/** Convert a native string to a unique string.
 */
EI_API const char *ei_token(const char *str);

/** Set value for a token storage.
 * \param str Must be a token already on local host.
 */
EI_API void ei_token_set(eiToken *s, const char *str);

/** Make a native string into a unique string, and get its hash.
 */
EI_API eiSizet ei_token_hash(const char *str);

#endif
