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

#ifndef EI_PLATFORM_H
#define EI_PLATFORM_H

/** Platform independent APIs for file I/O, multi-threading etc.
 * \file ei_platform.h
 * \author Elvic Liang
 * \def EI_DIR_SEPERATOR Users should always use '/' for any platforms.
 */

#include <ei_core.h>
#include <stdio.h>

/* Ensure MIN, MAX available */
#ifdef EI_OS_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* System configurations */
#define EI_MAX_MSG_GROUP_SIZE			1024
#define EI_MAX_FILE_NAME_LEN			512
#define EI_RENDER_THREAD_STACK_SIZE		(1024 * 1024 * 8)
#define EI_MAX_NUM_THREADS				64
#define EI_ALL_THREADS					(EI_MAX_NUM_THREADS * 2 + 1)
#define EI_STACK_OFFSET					1024
#define EI_CACHE_LINE_SIZE				128
#define EI_DIR_SEPERATOR				'/'

typedef void *	eiModuleHandle;
typedef void *	eiSymbolHandle;
typedef FILE *	eiFileHandle;
typedef eiInt	(*eiFileEnumerator)(const char* name, void* param, const eiBool is_dir);

typedef enum eiFileMode {
	EI_FILE_READ = 0,
	EI_FILE_WRITE,
	EI_FILE_APPEND,
	EI_FILE_READ_UPDATE,
	EI_FILE_WRITE_UPDATE,
	EI_FILE_APPEND_UPDATE,
} eiFileMode;

#ifdef EI_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define fseeko64	_fseeki64
#define ftello64	_ftelli64

#define EI_THREAD_FUNC			eiUint WINAPI
#define EI_THREAD_FUNC_RESULT	eiUint

typedef eiUint				eiTlsTag;
typedef HANDLE				eiThreadHandle;
typedef eiUint				(WINAPI *eiThreadFunction)(void *param);
typedef CRITICAL_SECTION	eiLock;
typedef void				eiEvent;

typedef struct eiFileMap {
	HANDLE		hFileMapping;
	void		*data;
	eiSizet		length;
} eiFileMap;

#else

#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64 1
#endif

#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64 1
#endif

#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>

#define EI_THREAD_FUNC			void *
#define EI_THREAD_FUNC_RESULT	void *

typedef pthread_key_t		eiTlsTag;
typedef pthread_t			eiThreadHandle;
typedef void *				(*eiThreadFunction)(void *param);
typedef pthread_mutex_t		eiLock;
typedef struct eiEvent {
	pthread_mutex_t		mutex;
	pthread_cond_t		condition;
	eiBool				signaled;
} eiEvent;

typedef struct eiFileMap {
	void		*padding;
	void		*data;
	eiUint64	length;
} eiFileMap;

#endif

/* Endianness */
enum {
	EI_LITTLE_ENDIAN = 0,
	EI_BIG_ENDIAN,
};

/** Get endianness of current platform.
 */
EI_API eiInt ei_get_endian();


/** Load a DSO by file path.
 */
EI_API eiModuleHandle ei_load_module(const char *path);

/** Get a symbol by name from a DSO.
 */
EI_API eiSymbolHandle ei_get_symbol(eiModuleHandle module, const char *name);

/** Unload a DSO.
 */
EI_API void ei_free_module(eiModuleHandle module);


/** Get the number of available threads.
 */
EI_API eiUint ei_get_number_threads();

/** Get the handle of the current thread in which this function is invoked.
 */
EI_API eiThreadHandle ei_get_current_thread();

/** Sleep current thread for a period in milliseconds.
 */
EI_API void ei_sleep(const eiUint milliseconds);

/** Fork a new thread.
 */
EI_API eiThreadHandle ei_create_thread(eiThreadFunction func, void *param, eiUint32 *tid);

/** Wait a thread to exit for infinite time.
 */
EI_API void ei_wait_thread(eiThreadHandle thread);

/** Wait multiple threads to exit for infinite time.
 */
EI_API void ei_wait_multiple_threads(const eiUint num_threads, eiThreadHandle threads[]);

/** Delete a thread.
 */
EI_API void ei_delete_thread(eiThreadHandle thread);


/** Initialize a mutex lock.
 */
EI_API void ei_lock_init(eiLock *lock);

/** Cleanup a mutex lock.
 */
EI_API void ei_lock_exit(eiLock *lock);

/** Create a mutex lock.
 */
EI_API eiLock *ei_create_lock();

/** Delete a mutex lock.
 */
EI_API void ei_delete_lock(eiLock *lock);

/** Try to acquire a mutex lock.
 */
EI_API eiBool ei_try_lock(eiLock *lock);

/** Acquire a mutex lock.
 */
EI_API void ei_lock(eiLock *lock);

/** Release a mutex lock.
 */
EI_API void ei_unlock(eiLock *lock);


/** Create an event.
 */
EI_API eiEvent *ei_create_event();

/** Delete an event.
 */
EI_API void ei_delete_event(eiEvent *e);

/** Signal an event, wake up waiting threads.
 */
EI_API void ei_signal_event(eiEvent *e);

/** Wait an event to be signaled for infinite time.
 */
EI_API void ei_wait_event(eiEvent *e);

/** Wait an event to be signaled with time-out in milliseconds.
 */
EI_API void ei_time_wait_event(eiEvent *e, const eiUint milliseconds);


/** Allocate a thread local storage (TLS) index.
 */
EI_API eiTlsTag ei_tls_create();

/** Set a value to a TLS slot of the calling thread by index.
 */
EI_API void ei_tls_set(eiTlsTag tag, void *value);

/** Get the value from a TLS slot of the calling thread by index.
 */
EI_API void *ei_tls_get(eiTlsTag tag);

/** Free a TLS index.
 */
EI_API void ei_tls_delete(eiTlsTag tag);


#ifdef _DEBUG
#define ei_allocate(size)					ei_allocate_dbg(size, (const char *)__FILE__, __LINE__)
#define ei_free(ptr)						ei_free_dbg(ptr, (const char *)__FILE__, __LINE__)
#define ei_reallocate(ptr, newsize)			ei_reallocate_dbg(ptr, newsize, (const char *)__FILE__, __LINE__)
#define ei_allocate_aligned(size)			ei_allocate_aligned_dbg(size, (const char *)__FILE__, __LINE__)
#define ei_free_aligned(ptr)				ei_free_aligned_dbg(ptr, (const char *)__FILE__, __LINE__)
#define ei_reallocate_aligned(ptr, newsize)	ei_reallocate_aligned_dbg(ptr, newsize, (const char *)__FILE__, __LINE__)
#else
#define ei_allocate(size)					ei_allocate_rel(size)
#define ei_free(ptr)						ei_free_rel(ptr)
#define ei_reallocate(ptr, newsize)			ei_reallocate_rel(ptr, newsize)
#define ei_allocate_aligned(size)			ei_allocate_aligned_rel(size)
#define ei_free_aligned(ptr)				ei_free_aligned_rel(ptr)
#define ei_reallocate_aligned(ptr, newsize)	ei_reallocate_aligned_rel(ptr, newsize)
#endif

#define EI_CHECK_FREE(p) \
	if ((p) != NULL) {\
		ei_free(p);\
		(p) = NULL;\
	}

#define EI_CHECK_FREE_ALIGNED(p) \
	if ((p) != NULL) {\
		ei_free_aligned(p);\
		(p) = NULL;\
	}

/** Allocate a block of memory on heap with debug information.
 */
EI_API void *ei_allocate_dbg(const eiUint size, const char *file, eiInt line);

/** Free the memory block allocated on heap with debug information.
 */
EI_API void ei_free_dbg(void *ptr, const char *file, eiInt line);

/** Re-allocate memory block to a new size on heap with debug information, 
 * existing content in the old block will be copied or truncated.
 */
EI_API void *ei_reallocate_dbg(void *ptr, const eiUint newsize, const char *file, eiInt line);

/** Allocate a block of memory on heap for release mode.
 */
EI_API void *ei_allocate_rel(const eiUint size);

/** Free the memory block allocated on heap for release mode.
 */
EI_API void ei_free_rel(void *ptr);

/** Re-allocate memory block to a new size on heap for release mode, 
 * existing content in the old block will be copied or truncated.
 */
EI_API void *ei_reallocate_rel(void *ptr, const eiUint newsize);

/** Allocate a block of memory on heap with debug information.
 */
EI_API void *ei_allocate_aligned_dbg(const eiUint size, const char *file, eiInt line);

/** Free the memory block allocated on heap with debug information.
 */
EI_API void ei_free_aligned_dbg(void *ptr, const char *file, eiInt line);

/** Re-allocate memory block to a new size on heap with debug information, 
 * existing content in the old block will be copied or truncated.
 */
EI_API void *ei_reallocate_aligned_dbg(void *ptr, const eiUint newsize, const char *file, eiInt line);

/** Allocate a block of memory on heap for release mode.
 */
EI_API void *ei_allocate_aligned_rel(const eiUint size);

/** Free the memory block allocated on heap for release mode.
 */
EI_API void ei_free_aligned_rel(void *ptr);

/** Re-allocate memory block to a new size on heap for release mode, 
 * existing content in the old block will be copied or truncated.
 */
EI_API void *ei_reallocate_aligned_rel(void *ptr, const eiUint newsize);


/** Get the current time in milliseconds.
 */
EI_API eiInt ei_get_time();


/** Get the value of an environment variable by name.
 */
EI_API void ei_get_environment(char* str, const char* name);


/** Check whether a file exsits by name.
 */
EI_API eiBool ei_file_exists(const char* path);

/** Open a file by name.
 */
EI_API eiFileHandle ei_open_file(const char* path, const eiFileMode mode);

/** Read a file.
 */
EI_API eiSizet ei_read_file(eiFileHandle file, void* buf, const eiSizet size);

/** Write a file.
 */
EI_API eiSizet ei_write_file(eiFileHandle file, const void* buf, const eiSizet size);

/** Seek a file.
 */
EI_API eiInt64 ei_seek_file(eiFileHandle file, const eiInt64 offset);

/** Get the offset of current file pointer.
 */
EI_API eiInt64 ei_get_file_offset(eiFileHandle file);

/** Close a file.
 */
EI_API void ei_close_file(eiFileHandle file);

/** Delete a file by name.
 */
EI_API void ei_delete_file(const char* path);

/** Generate a full path to a temporary file in specified directory.
 */
EI_API void ei_get_temp_filename(char *result, const char* dir, const char* prefix);

/** Append file name to a specified directory.
 */
EI_API void ei_append_filename(char* result, const char* dir, const char* filename);


/** Create a new directory.
 */
EI_API void ei_create_directory(const char* dir);

/** Remove a directory.
 */
EI_API void ei_delete_directory(const char* dir);

/** Enumerate all files and directories in a directory.
 */
EI_API void ei_enumerate(const char* dir, eiFileEnumerator func, void* param);

/** Get current directory of the executable. the returned string is in UTF-8 format.
 */
EI_API void ei_get_current_directory(char* dir);

/** Split name into partA[splitter]partB format.
 */
EI_API void ei_split_name(
	char *partA, 
	char *partB, 
	const char *name, 
	const eiSizet max_len, 
	const char splitter);

/** Extract the file extension from a file name.
 */
EI_API void ei_get_file_extension(char* ext, const char* name);

/** Get file length.
 */
EI_API eiUint64 ei_get_file_length(eiFileHandle file);

/** Extend or truncate a file to a new size.
 */
EI_API void ei_truncate_file(eiFileHandle file, const eiUint64 size);


/** Get the page size of current system.
 */
EI_API eiSizet ei_get_page_size();

/** Map a portion of a file into memory.
 */
EI_API void ei_map_file(eiFileMap* file_map, eiFileHandle file, const eiFileMode mode, const eiUint64 offset, const eiSizet length);

/** Unmap a portion of a file from memory.
 */
EI_API void ei_unmap_file(eiFileMap* file_map);

/** Set thread to low priority.
 */
EI_API void ei_set_low_thread_priority(eiThreadHandle thread);

#endif
