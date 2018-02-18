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

#ifndef EI_DATAFLOWX_H
#define EI_DATAFLOWX_H

/** C++ interfaces of dataflow system.
 * \file ei_dataflowx.h
 * \author Elvic Liang
 */

#include <ei_dataflow.h>

/** The accessor for data items stored in database.
 */
template <typename T>
class eiDataAccessor
{
public:
	inline eiDataAccessor(const eiTag tag)
	{
		m_tag = tag;
		m_ptr = (T *)ei_db_access(tag);
	}

	inline ~eiDataAccessor()
	{
		ei_db_end(m_tag);
	}

	inline T *operator -> () const
	{
		return m_ptr;
	}

	inline T *get() const
	{
		return m_ptr;
	}

private:
	eiTag			m_tag;
	T				*m_ptr;
};

/** The deferred accessor which will defer the data 
 * initialization to the next time of access.
 */
template <typename T>
class eiDeferDataAccessor
{
public:
	inline eiDeferDataAccessor(const eiTag tag)
	{
		m_tag = tag;
		m_ptr = (T *)ei_db_access_defer(tag);
	}

	inline ~eiDeferDataAccessor()
	{
		ei_db_end_defer(m_tag);
	}

	inline T *operator -> () const
	{
		return m_ptr;
	}

	inline T *get() const
	{
		return m_ptr;
	}

private:
	eiTag			m_tag;
	T				*m_ptr;
};

/** Get statically cached thread count.
 */
EI_API eiInt ei_get_static_thread_count();

/** The utility class for implementing parallel for
 */
template <eiInt STEP, typename FUNC>
struct eiRunner
{
	FUNC &		m_func;

	struct eiRunTask
	{
		eiTask	task;
		FUNC	*func;
		eiInt	begin;
		eiInt	end;
	};

	static eiBool task_execute(eiTask *task, void *param)
	{
		eiRunTask *run_task = (eiRunTask *)task;
		FUNC *func = run_task->func;
		const eiInt begin = run_task->begin;
		const eiInt end = run_task->end;

		for (eiInt i = begin; i < end; i += STEP)
		{
			(*func)(i);
		}

		return EI_TRUE;
	}

	static void task_deletethis(eiTask *task)
	{
		/* nothing to do */
	}

	inline eiRunner(
		eiInt begin, 
		eiInt end, 
		FUNC & func)
		: m_func(func)
	{
		if (end <= begin)
		{
			return;
		}

		eiRunTask tasks[EI_MAX_NUM_THREADS];

		eiInt task_count = end - begin;
		/* round block_size to the next multiply of STEP */
		eiInt block_size = STEP * lceilf(
			(eiScalar)task_count / (eiScalar)ei_get_static_thread_count() / (eiScalar)STEP);
		eiInt thread_count = clamp(task_count / block_size, 1, ei_get_static_thread_count());
		eiInt block_begin = begin;
		eiInt block_end = begin + block_size;

		for (eiInt i = 0; i < thread_count; ++i)
		{
			eiRunTask *run_task = &tasks[i];

			ei_task_init(&(run_task->task), task_execute, task_deletethis);

			run_task->func = &m_func;
			run_task->begin = block_begin;
			if (i == thread_count - 1)
			{
				run_task->end = end;
			}
			else
			{
				run_task->end = block_end;
			}

			ei_task_spawn(&(run_task->task));

			block_begin += block_size;
			block_end += block_size;
		}

		ei_task_wait();
	}
};

/** Parallel for utility using task scheduler in the form:
 * for (eiInt i = begin; i < end; ++i)
 */
template <typename FUNC>
inline void ei_parallel_for(
	eiInt begin, 
	eiInt end, 
	FUNC & func)
{
	eiRunner<1, FUNC> runner(begin, end, func);
}

/** Parallel for utility using task scheduler in the form:
 * for (eiInt i = begin; i < end; i += STEP)
 */
template <eiInt STEP, typename FUNC>
inline void ei_parallel_for(
	eiInt begin, 
	eiInt end, 
	FUNC & func)
{
	eiRunner<STEP, FUNC> runner(begin, end, func);
}

#endif
