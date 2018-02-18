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

#ifndef EI_BTREE_H
#define EI_BTREE_H

/** Balanced binary search tree.
 * \file ei_btree.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_assert.h>

/** Declaration of node of balanced binary search tree.
 */
typedef struct ei_btree_node ei_btree_node;

/** Declaration of balanced binary search tree.
 */
typedef struct ei_btree ei_btree;

/** Comparing callback for sorting nodes in ascending order, 
 * this function should return < 0 if lhs < rhs, 
 * return == 0 if lhs == rhs, 
 * return > 0 if lhs > rhs.
 */
typedef eiIntptr (*ei_btree_compare)(ei_btree_node *lhs, ei_btree_node *rhs, void *param);

/** Node definition of balanced binary search tree
 */
struct ei_btree_node {
	struct ei_btree_node	*left;		/**< left child */
	struct ei_btree_node	*right;		/**< right child */
	eiInt					height;		/**< height of the subtree, do not modify this field */
};

/** The function callback for deleting a node.
 */
typedef void (*ei_btree_delete_node)(ei_btree_node *node, void *param);

/** The procedure used to traverse the tree, if zero is 
 * returned while traversing nodes, the traversal will be terminated.
 */
typedef eiInt (*ei_btree_proc)(ei_btree_node *node, void *param);

/** Balanced binary search tree definition
 */
struct ei_btree {
	ei_btree_node			*root;			/**< root of the tree */
	ei_btree_compare		compare;		/**< compare callback */
	ei_btree_delete_node	delete_node;	/**< delete node callback */
	eiSizet					size;			/**< number of nodes */
	void					*delete_node_param;
};

/** Initialize the node.
 */
EI_API void ei_btree_node_init(ei_btree_node *node);

/** Clear the node.
 */
EI_API void ei_btree_node_clear(ei_btree_node *node);

/** Initialize a tree.
 */
EI_API void ei_btree_init(ei_btree *tree, ei_btree_compare compare, ei_btree_delete_node delete_node, void *delete_node_param);

/** Clear a tree.
 */
EI_API void ei_btree_clear(ei_btree *tree);

/** This macro allows us to inline compare function for faster code in C.
 */
#define EI_DEFINE_BTREE_LOOKUP(ClassName, CompareFuncName) \
inline ClassName *ei_btree_lookup_##ClassName(ei_btree *tree, ClassName *node, void *param)\
{\
	ei_btree_node *p = NULL;\
	eiIntptr result;\
	\
	p = tree->root;\
	\
	while (p != NULL)\
	{\
		result = CompareFuncName((ei_btree_node *)node, p, param);\
		\
		if (result == 0)\
		{\
			return (ClassName *)p;\
		}\
		else if (result < 0)\
		{\
			p = p->left;\
		}\
		else\
		{\
			p = p->right;\
		}\
	}\
	\
	return NULL;\
}

/** Lookup a node by key, and return the actual node in the tree. 
 * you can fill up the input node with only key field, leaving the 
 * data field empty.
 */
inline ei_btree_node *ei_btree_lookup(ei_btree *tree, ei_btree_node *node, void *param)
{
	ei_btree_node *p = NULL;
	eiIntptr result;

	EI_DBG_ASSERT(tree != NULL && tree->compare != NULL);
	EI_DBG_ASSERT(node != NULL);

	p = tree->root;

	while (p != NULL)
	{
		result = tree->compare(node, p, param);

		if (result == 0)
		{
			return p;
		}
		else if (result < 0)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}

	return NULL;
}

/** Insert a node into a tree, if the key already exists in the 
 * tree, insertion will not be performed.
 */
EI_API void ei_btree_insert(ei_btree *tree, ei_btree_node *node, void *param);

/** Delete a node from a tree.
 */
EI_API void ei_btree_delete(ei_btree *tree, ei_btree_node *node, void *param);

/** Traverse all nodes in the tree. The traversal order is not 
 * guaranteed to be increasing regarding the key.
 */
EI_API void ei_btree_traverse(ei_btree *tree, ei_btree_proc proc, void *param);

/** Return the number of nodes in this tree.
 */
EI_API eiSizet ei_btree_size(ei_btree *tree);

#endif
