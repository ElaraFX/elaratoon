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

#ifndef EI_LIST_H
#define EI_LIST_H

/** Intrusive non-thread-safe double linked list.
 * \file ei_list.h
 * \author Elvic Liang
 *
 * "Intrusive" means this container does not carry data as the member 
 * of its node, users must allocate/free the nodes on their own.
 */

#include <ei_core.h>
#include <ei_assert.h>

/** Declaration of the node of double linked list.
 */
typedef struct ei_list_node ei_list_node;

/** Declaration of double linked list.
 */
typedef struct ei_list ei_list;

/** ei_list_node should be part of user's data structure.
 */
struct ei_list_node {
	struct ei_list_node		*prev;
	struct ei_list_node		*next;
};

/** The function callback for deleting a node in a list.
 */
typedef void (*ei_list_delete_node)(ei_list_node *node);

/** The generic double link list container.
 */
struct ei_list {
	ei_list_node		head;
	ei_list_node		*tail;
	eiSizet				size;
	ei_list_delete_node	delete_node;
};

/** Initialize the node.
 */
EI_API void ei_list_node_init(ei_list_node *node);

/** Clear the node.
 */
EI_API void ei_list_node_clear(ei_list_node *node);

/** Initialize a double linked list.
 */
EI_API void ei_list_init(ei_list *list, ei_list_delete_node delete_node);

/** Get the number of nodes in a double linked list.
 */
inline eiSizet ei_list_size(const ei_list* list)
{
	EI_DBG_ASSERT(list != NULL);

	return list->size;
}

/** Returns whether a list is empty.
 */
inline eiBool ei_list_empty(const ei_list *list)
{
	EI_DBG_ASSERT(list != NULL);

	return (list->size == 0);
}

/** Returns whether the list contains a specific node.
 */
inline eiBool ei_list_contains(const ei_list *list, const ei_list_node *node)
{
	/* a node belongs to this list only when its prev is non-null */
	EI_DBG_ASSERT(list != NULL && node != NULL);

	return (node->prev != NULL);
}

/** Insert a new node after an existing node.
 */
EI_API void ei_list_insert(ei_list *list, ei_list_node *pnode, ei_list_node *newnode);

/** Push a node at the end of the list.
 */
EI_API void ei_list_push_back(ei_list *list, ei_list_node *node);

/** Push a node at the beginning of the list.
 */
EI_API void ei_list_push_front(ei_list *list, ei_list_node *node);

/** Move a node to the end of the list.
 */
inline void ei_list_move_back(ei_list *list, ei_list_node *node)
{
	EI_DBG_ASSERT(list != NULL && node != NULL);

	/* move the node to back when it belongs to this list */
	if (!ei_list_contains(list, node))
	{
		ei_list_push_back(list, node);
		return;
	}

	if (node != list->tail)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;

		node->prev = list->tail;
		node->next = NULL;

		list->tail->next = node;
		list->tail = node;
	}
}

/** Pop a node from the list.
 */
EI_API void ei_list_pop(ei_list *list, ei_list_node *pnode);

/** Pop a node from the list, then call delete_node callback on the node.
 */
EI_API void ei_list_erase(ei_list *list, ei_list_node *node);

/** Get the beginning of the list.
 */
EI_API ei_list_node *ei_list_begin(ei_list *list);

/** Get the end of the list.
 */
EI_API ei_list_node *ei_list_end(ei_list *list);

/** Get the first node in the list.
 */
EI_API ei_list_node *ei_list_front(ei_list *list);

/** Get the last node in the list.
 */
EI_API ei_list_node *ei_list_back(ei_list *list);

/** Clear a double linked list.
 */
EI_API void ei_list_clear(ei_list *list);

#endif
