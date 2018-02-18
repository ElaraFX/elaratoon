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

#ifndef EI_RWLOCK_H
#define EI_RWLOCK_H

/** A lock-free implementation of reader-writer lock.
 * \file ei_rwlock.h
 */

#include <ei_core.h>
#include <ei_platform.h>
#include <ei_atomic.h>
#include <ei_backoff.h>
#include <ei_assert.h>

/** Reader-writer lock state structure.
 */
typedef struct eiRWLock {
	eiAtomic	ref_count;
} eiRWLock;

/** Initialize a reader-writer lock.
 */
EI_API void ei_rwlock_init(eiRWLock *lock);

/** Cleanup a reader-writer lock.
 */
EI_API void ei_rwlock_exit(eiRWLock *lock);

/** Create a reader-writer lock.
 */
EI_API eiRWLock *ei_create_rwlock();

/** Delete a reader-writer lock.
 */
EI_API void ei_delete_rwlock(eiRWLock *lock);

/** Acquire a lock for reading.
 */
inline void ei_read_lock(eiRWLock *lock)
{
	eiInt result;

	EI_DBG_ASSERT(lock != NULL);

	for (;;)
	{
		result = ei_atomic_inc(&lock->ref_count);

		if (result > 0)
		{
			return;
		}
		else
		{
			eiBackoff backoff;

			ei_backoff_reset(&backoff);

			while (ei_atomic_read(&lock->ref_count) < 0)
			{
				ei_backoff_pause(&backoff);
			}
		}
	}
}

/** Release a lock for reading.
 */
inline void ei_read_unlock(eiRWLock *lock)
{
	EI_DBG_ASSERT(lock != NULL);

	ei_atomic_dec(&lock->ref_count);
}

/** Try to acquire a lock for writing, returns 
 * whether the lock has successfully been acquired.
 */
EI_API eiBool ei_try_write_lock(eiRWLock *lock);

/** Acquire a lock for writing.
 */
EI_API void ei_write_lock(eiRWLock *lock);

/** Release a lock for writing.
 */
EI_API void ei_write_unlock(eiRWLock *lock);

/** Downgrade a writer lock to reader lock.
 */
EI_API void ei_downgrade_lock(eiRWLock *lock);

/** Is the lock in locked state for reading.
 */
EI_API eiBool ei_is_read_locked(eiRWLock *lock);

/** Is the lock in locked state for writing.
 */
EI_API eiBool ei_is_write_locked(eiRWLock *lock);

/** Is the lock in locked state for either 
 * reading or writing.
 */
EI_API eiBool ei_is_locked(eiRWLock *lock);

#endif
