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

#ifndef EI_DATAFLOW_H
#define EI_DATAFLOW_H

/** Data flow architecture for deferred loading and lazy evaluation.
 * \file ei_dataflow.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_platform.h>
#include <ei_atomic.h>
#include <ei_ts_queue.h>
#include <ei_vector.h>

#define EI_MAX_TLS_INTERFACE_SIZE		8

/** The job state */
enum {
	EI_JOB_FAILED = 0,		/**< failed in execution */
	EI_JOB_SUCCEEDED,		/**< successfully executed */
	EI_JOB_STARTED,			/**< started execution */
	EI_JOB_CANCELLED,		/**< cancelled by user */
};

typedef struct eiTLS			eiTLS;
typedef struct eiThreadInfo		eiThreadInfo;
typedef struct eiProcess		eiProcess;
typedef struct eiTask			eiTask;

/** The callback for initializing custom thread local storage.
 */
typedef void (*eiInitTLS)(eiTLS *tls);
/** The callback for cleaning up custom thread local storage.
 */
typedef void (*eiExitTLS)(eiTLS *tls);
/** The callback for clear cache in thread local storage.
 */
typedef void (*eiTLSClearCache)(eiTLS *tls);

/** The info of a managed data item */
struct eiData {
	void				*ptr;				/**< the address of the data in local memory */
	eiAtomic			init_flag;
	eiUint				size;
	eiByte				type;
	eiByte				padding[3];
	eiAtomic			initing_thread;		/**< thread which is currently initializing this data */
	eiAtomic			ref_count;			/**< the reference count for scene graph, will not be modified by the database */
	eiRWLock			lock;				/**< the lock to prevent from concurrent updating */
};

/** Generate data callback.
 * most of the rendering data should have their data generators, which access 
 * multiple input data, then generate single output data. the data generator 
 * is only called once to initialize the data at the first time when we access 
 * the data, so most of the rendering data are generated on-demand, deferred 
 * to the first access time.
 */
typedef void (*ei_db_generate_data)(const eiTag data_tag, void *pData, eiTLS *pTls);
/** The data destructor which will be called when deleting the data.
 * we should delete all data allocated in generate_data using local pointers 
 * in this callback. notice that this function will be called multiple times 
 * for all local hosts for each data item, do not delete data referenced by 
 * tags because database will handle that automatically.
 */
typedef void (*ei_db_clear_data)(void *pData);
/** The class destructor called automatically by the system.
 */
typedef void (*ei_db_delete_data)(void *pData);
/** Execute job callback.
 */
typedef eiInt (*ei_job_execute)(
	eiThreadInfo *pThreadInfo, 
	void *job, 
	void *param);

/** Data generator description, including necessary callbacks.
 */
struct eiDataGen {
	ei_db_generate_data		generate_data;
	ei_db_clear_data		clear_data;
	ei_db_delete_data		delete_data;
	ei_job_execute			execute_job;		/**< the job related functions will be available 
												     when the data is a job */
	eiSizet					type_size;			/**< the size of this data type when using it as element 
												     of data array or data table, etc. for dynamically 
												     growing data, this size is meaningless, and we should 
												     never use varying-sized data as data array element. */
	eiAtomicptr				item_count;
	eiAtomicptr				item_peak;
	eiAtomicptr				mem_size;
	eiAtomicptr				mem_peak;
};

/** The table of data generators, user must fill this.
 */
struct eiDataGenTable {
	eiDataGen		*data_gens;
	eiUint			num_data_gens;
};

/** Startup the global database for current session.
 */
EI_API void ei_db_startup();

/** Shutdown the global databse for current session.
 */
EI_API void ei_db_shutdown();

/** Perform garbage collection, delete all unreleased data.
 */
EI_API void ei_db_gc();

/** Get data element size in bytes by type.
 */
EI_API eiSizet ei_db_type_size(const eiInt type);

/** Create a data item in the database.
 * \param tag The tag returned to identify the created item.
 * \param type The data type.
 * \param size The size in bytes.
 */
EI_API void *ei_db_create(
	eiTag * const tag, 
	const eiInt type, 
	eiUint size);

/** Resize a data item.
 * \param tag The tag of the data item.
 * \param newsize The new size in bytes.
 */
EI_API void *ei_db_resize(
	const eiTag tag, 
	eiUint newsize);

/** Get lock for a specific data item by tag.
 * \param tag The tag of the data item.
 */
EI_API eiRWLock *ei_db_get_lock(const eiTag tag);

/** Begin accessing a data item by tag.
 * \param tag The tag of the data item.
 */
EI_API void *ei_db_access(const eiTag tag);

/** End accessing a data item by tag.
 * \param tag The tag of the data item.
 */
EI_API void ei_db_end(const eiTag tag);

/** Begin accessing a data item by tag without affecting the 
 * initialization state, if the initialization is needed, it will be 
 * deferred to the next access time.
 * The caller must use ei_db_end_defer to finish accessing.
 * \param tag The tag of the data item.
 */
EI_API void *ei_db_access_defer(const eiTag tag);

/** End accessing a data item by tag which was deferred accessed.
 * \param tag The tag of the data item.
 */
EI_API void ei_db_end_defer(const eiTag tag);

/** Add reference to a data item without actual access to the data content, 
 * return the current reference count of the data item after the referencing. 
 * this function is designed for reference counted life-time management.
 */
EI_API eiInt ei_db_ref(const eiTag tag);

/** Release reference to a data item without actual access to the data 
 * content, return the current reference count of the data item after the 
 * unreferencing. this function is designed for reference counted life-time 
 * management.
 */
EI_API eiInt ei_db_unref(const eiTag tag);

/** Return the current reference count of the data item. this function 
 * is designed for reference counted life-time management.
 */
EI_API eiInt ei_db_getref(const eiTag tag);

/** Delete a data item by tag.
 * \param tag The tag of the data item.
 */
EI_API void ei_db_delete(const eiTag tag);

/** Dirt a data item by tag, the data item will be re-initialized. all data 
 * in scene graph which have been changed by scene description APIs should 
 * be invalidated using this function, and all generated data from these 
 * changed data should be invalidated too.
 * \param tag The tag of the data item to be flagged dirty.
 */
EI_API void ei_db_dirt(const eiTag tag);

/** Get the type of a data by tag.
 * \param tag The tag of the data item whose type will be returned.
 */
EI_API eiInt ei_db_type(const eiTag tag);

/** Get the size of a data by tag. This function can be very slow, try 
 * to avoid using it in multi-threaded environment.
 * \param tag The tag of the data item whose size will be returned.
 */
EI_API eiUint ei_db_size(const eiTag tag);

/** Get physical memory usage peak in bytes.
 */
EI_API eiIntptr ei_db_mem_peak();

/** Dump statistics on memory usage of all data items.
 */
EI_API void ei_db_dump();

EI_API eiInterface ei_tls_get_interface(eiTLS *tls, const eiUint interfaceId);
EI_API void ei_tls_set_interface(eiTLS *tls, const eiUint interfaceId, eiInterface iface);
EI_API void ei_tls_free_interface(eiTLS *tls, const eiUint interfaceId);
EI_API void *ei_tls_allocate(eiTLS *tls, eiSizet size);
EI_API void ei_tls_free(eiTLS *tls, void *ptr);

EI_API eiThreadID ei_tls_get_thread_id(eiTLS *tls);

/** The callback for being notified that one pass started.
 */
typedef void (*ei_process_pass_started)(eiProcess *process, eiInt pass_id);
/** The callback for being notified that one pass finished.
 */
typedef void (*ei_process_pass_finished)(eiProcess *process, eiInt pass_id);
/** The callback for being notified that one job started.
 */
typedef void (*ei_process_job_started)(
	eiProcess *process, 
	const eiTag job, 
	const eiThreadID threadId);
/** The callback for being notified that one job finished.
 */
typedef void (*ei_process_job_finished)(
	eiProcess *process, 
	const eiTag job, 
	const eiInt job_state, 
	const eiThreadID threadId);
/** The callback for displaying job processing specific information.
 */
typedef void (*ei_process_info)(
	eiProcess *process, 
	const char *msg);

/** The description of a rendering process.
 * users can override the callbacks to handle certain 
 * rendering messages.
 */
struct eiProcess {
	ei_process_pass_started		pass_started;
	ei_process_pass_finished	pass_finished;
	ei_process_job_started		job_started;
	ei_process_job_finished		job_finished;
	ei_process_info				info;
};

/** The process must be set before running process.
 */
EI_API void ei_job_set_process(eiProcess *pProcess);

/** Create worker threads.
 * \param nthreads The number of threads to launch on local render 
 * master for processing rendering jobs. if value <= 0 is specified, 
 * the render master will determine the number automatically.
 * \return The number of successfully created threads.
 */
EI_API eiUint ei_job_create_workers(const eiInt nthreads, eiBool low_thread_priority);

/** Delete all worker threads.
 */
EI_API void ei_job_delete_workers();

/** Schedule a job for parallel execution.
 */
EI_API void ei_job_add(const eiTag tag);

/** Increment job progress.
 */
EI_API void ei_job_progress(const eiUlong count);

/** Reset job progress, and set total job amount for all succeeding runs.
 */
EI_API void ei_job_reset_progress(const eiUlong total_amount);

/** Start processing scheduled jobs until 
 * all jobs are completed.
 * The formatted string will be displayed in client application.
 */
EI_API void ei_job_run(eiInt pass_id, const char *format, ...);

/** Test if current rendering is aborted.
 */
EI_API eiBool ei_job_aborted();

/** Call this function to abort rendering on all hosts.
 */
EI_API void ei_job_abort(const eiBool value);

/** Execute a job directly in the context of current thread.
 * this function does not delete the executed job.
 */
EI_API void ei_job_do(const eiTag job, void *param);

/** Display job processing specific messages on client application.
 */
EI_API void ei_job_info(const char* format, ...);

EI_API eiTLS *ei_job_get_tls();
EI_API eiInt ei_job_get_num_finished_jobs();
EI_API eiInt ei_job_get_num_failed_jobs();
EI_API eiUint ei_job_get_num_threads();

/** Get the completed percentage of current running jobs.
 */
EI_API eiGeoScalar ei_job_get_percent();

/** User threads must call this function before calling any database functions.
 * the main thread that initializes this library does not need to call this 
 * function.
 */
EI_API void ei_job_register_thread();
/** User threads should call this function before finishing execution.
 * the main thread that initializes this library does not need to call this 
 * function.
 */
EI_API void ei_job_unregister_thread();

typedef void (*eiTraverseTLSCallback)(eiTLS *tls, void *param);
EI_API void ei_job_traverse_tls(eiTraverseTLSCallback cb, void *param);

typedef eiBool (*ei_task_execute)(eiTask *task, void *param);
typedef void (*ei_task_deletethis)(eiTask *task);

/** A small task which can spawn child tasks.
 */
struct eiTask {
	union {
		struct {
			ei_ts_queue_node	node;			/**< the node in task queue */
			ei_task_execute		execute;		/**< the callback to execute this task */
			ei_task_deletethis	deletethis;		/**< the callback to delete this task */
			eiTask				*parent;		/**< the task whose reference count includes me */
			eiAtomic			ref_count;		/**< reference count used for synchronization */
			eiThreadID			threadId;		/**< the thread which allocated this task */
			eiBool				abortable;		/**< if this task can respond to ei_job_abort */
		};
		eiUintptr				padding[8];		/**< pad to 16 byte boundary for SSE */
	};
};

EI_API void ei_task_init(
	eiTask *task, 
	ei_task_execute execute, 
	ei_task_deletethis deletethis);
EI_API void ei_task_exit(eiTask *task);

/** Allocate a block of memory for task.
 */
EI_API eiTask *ei_task_allocate(const eiUint size);
/** Free a block of memory for task.
 */
EI_API void ei_task_free(eiTask *task);
/** Get the current running task, which can be used as the parent task 
 * of spawned child tasks.
 */
EI_API eiTask *ei_task_current();
/** Spawn a new task, as the child task of a specified task.
 */
EI_API void ei_task_spawn_from(eiTask *task, eiTask *parent);
/** Spawn a new task, as the child task of currently running task.
 */
EI_API void ei_task_spawn(eiTask *task);
/** Execute a task directly in the context of current thread.
 */
EI_API void ei_task_do(eiTask *task, void *param);
/** Try to execute a task.
 */
EI_API eiBool ei_task_run();
/** Wait until all child tasks completed.
 */
EI_API void ei_task_wait();

#endif
