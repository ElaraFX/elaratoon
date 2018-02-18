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

#ifndef EI_SLIST_H
#define EI_SLIST_H

/** Intrusive non-thread-safe single linked list.
 * \file ei_slist.h
 * \author Elvic Liang
 *
 * This container is very similar to ei_list, except that it's 
 * single linked, which saves memory of one pointer.
 */

#include <ei_core.h>
#include <ei_assert.h>

/** The node in single linked list.
 */
typedef struct ei_slist_node {
	struct ei_slist_node	*next;
} ei_slist_node;

/** The function callback for deleting a node in a list.
 */
typedef void (*ei_slist_delete_node)(ei_slist_node *node, void *param);

/** The generic single linked list container.
 */
typedef struct ei_slist {
	ei_slist_node			head;
	ei_slist_node			*tail;
	eiSizet					size;
	ei_slist_delete_node	delete_node;
} ei_slist;

/** Initialize the node.
 */
EI_API void ei_slist_node_init(ei_slist_node *node);

/** Clear the node.
 */
EI_API void ei_slist_node_clear(ei_slist_node *node);

/** Initialize a single linked list.
 */
EI_API void ei_slist_init(ei_slist *slist, ei_slist_delete_node delete_node);

/** Returns the number of nodes in a list.
 */
inline eiSizet ei_slist_size(const ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return slist->size;
}

/** Returns whether the list is empty.
 */
inline eiBool ei_slist_empty(const ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return (slist->size == 0);
}

/** Insert a new node after an existing node.
 */
EI_API void ei_slist_insert(ei_slist *list, ei_slist_node *pnode, ei_slist_node *newnode);

/** Push a node at the end of the list.
 */
EI_API void ei_slist_push_back(ei_slist *slist, ei_slist_node *node);

/** Push a node at the beginning of the list.
 */
EI_API void ei_slist_push_front(ei_slist *slist, ei_slist_node *node);

/** Pop a node after an existing node.
 */
EI_API ei_slist_node *ei_slist_pop(ei_slist *list, ei_slist_node *pnode_prev);

/** Pop a node after an existing node, then call delete_node callback on the node.
 */
EI_API void ei_slist_erase(ei_slist *list, ei_slist_node *pnode_prev, void *param);

/** Get the beginning of the list.
 */
inline ei_slist_node *ei_slist_begin(ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return slist->head.next;
}

/** Get the end of the list.
 */
inline ei_slist_node *ei_slist_end(ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return &slist->head;
}

/** Get the first node in the list.
 */
inline ei_slist_node *ei_slist_front(ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return slist->head.next;
}

/** Get the last node in the list.
 */
inline ei_slist_node *ei_slist_back(ei_slist *slist)
{
	EI_DBG_ASSERT(slist != NULL);

	return slist->tail;
}

/** Clear a single linked list.
 */
EI_API void ei_slist_clear(ei_slist *slist, void *param);

#endif
