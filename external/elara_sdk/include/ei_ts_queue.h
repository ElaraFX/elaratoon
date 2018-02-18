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

#ifndef EI_TS_QUEUE_H
#define EI_TS_QUEUE_H

/** A thread-safe queue container.
 * \file ei_ts_queue.h
 */

#include <ei_list.h>
#include <ei_rwlock.h>

/** Thread-safe queue structure.
 */
struct ei_ts_queue
{
	ei_list			q;
	eiRWLock		*lock;
};

typedef ei_list_node			ei_ts_queue_node;
typedef ei_list_delete_node		ei_ts_queue_delete_node;

/** Initialize the node.
 */
EI_API void ei_ts_queue_node_init(ei_ts_queue_node *node);

/** Clear the node.
 */
EI_API void ei_ts_queue_node_clear(ei_ts_queue_node *node);

/** Initialize a thread-safe queue.
 */
EI_API void ei_ts_queue_init(ei_ts_queue *queue, ei_ts_queue_delete_node delete_node);

/** Cleanup a thread-safe queue.
 */
EI_API void ei_ts_queue_exit(ei_ts_queue *queue);

/** Clear a thread-safe queue.
 */
EI_API void ei_ts_queue_clear(ei_ts_queue *queue);

/** Push a node into the queue.
 */
EI_API void ei_ts_queue_push(ei_ts_queue *queue, ei_ts_queue_node *node);

/** Pop a node from the queue, returns NULL if the queue is empty.
 */
EI_API ei_ts_queue_node *ei_ts_queue_pop(ei_ts_queue *queue);

/** Pop a node from bottom of the queue, returns NULL if the queue is empty.
 */
EI_API ei_ts_queue_node *ei_ts_queue_pop_bottom(ei_ts_queue *queue);

/** Try to pop a node from the queue, returns NULL if the queue 
 * is empty or the try failed.
 */
EI_API ei_ts_queue_node *ei_ts_queue_try_pop(ei_ts_queue *queue);

/** Try to pop a node from bottom of the queue, returns NULL if 
 * the queue is empty or the try failed.
 */
EI_API ei_ts_queue_node *ei_ts_queue_try_pop_bottom(ei_ts_queue *queue);

#endif
