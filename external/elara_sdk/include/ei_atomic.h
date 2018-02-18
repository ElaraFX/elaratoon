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

#ifndef EI_ATOMIC_H
#define EI_ATOMIC_H

/** Atomic data types and operations.
 * \file ei_atomic.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <xmmintrin.h>

typedef struct eiAtomic {
	volatile long v;
} eiAtomic;

#ifdef EI_ARCH_X64

typedef struct eiAtomic64 {
	volatile long long v;
} eiAtomic64;

#endif

#ifdef EI_OS_WINDOWS

#include <intrin.h>
#include <windows.h>

inline int __bsf(int v)
{
	unsigned long r = 0;
	_BitScanForward(&r, v);
	return r;
}

inline int __btc(int v, int i)
{
	long r = v;
	_bittestandcomplement(&r, i);
	return r;
}

inline void ei_yield()
{
	SwitchToThread();
}

/** This function returns the initial value of destination parameter.
 */
inline eiInt ei_atomic_add(eiAtomic *destination, eiInt value)
{
	return _InterlockedExchangeAdd(&destination->v, value);
}

/** This function returns the resulting incremented value.
 */
inline eiInt ei_atomic_inc(eiAtomic *destination)
{
	return _InterlockedIncrement(&destination->v);
}

/** This function returns the resulting decremented value.
 */
inline eiInt ei_atomic_dec(eiAtomic *destination)
{
	return _InterlockedDecrement(&destination->v);
}

/** This function returns the initial value of destination parameter.
 */
inline eiInt ei_atomic_swap(eiAtomic *destination, eiInt value)
{
	return _InterlockedExchange(&destination->v, value);
}

/** This function returns the initial value of destination parameter.
 */
inline eiInt ei_atomic_cas(eiAtomic *destination, eiInt exchange, eiInt comperand)
{
	return _InterlockedCompareExchange(&destination->v, exchange, comperand);
}

/** This function returns the initial value of destination parameter.
 */
inline eiInt ei_atomic_clear_mask(eiAtomic *destination, eiInt value)
{
	return _InterlockedAnd(&destination->v, ~value);
}

/** This function returns the initial value of destination parameter.
 */
inline eiInt ei_atomic_set_mask(eiAtomic *destination, eiInt value)
{
	return _InterlockedOr(&destination->v, value);
}

#ifdef EI_ARCH_X64

/** This function returns the initial value of destination parameter.
 */
inline eiInt64 ei_atomic_add64(eiAtomic64 *destination, eiInt64 value)
{
	return _InterlockedExchangeAdd64(&destination->v, value);
}

inline eiInt64 ei_atomic_inc64(eiAtomic64 *destination)
{
	return _InterlockedIncrement64(&destination->v);
}

inline eiInt64 ei_atomic_dec64(eiAtomic64 *destination)
{
	return _InterlockedDecrement64(&destination->v);
}

inline eiInt64 ei_atomic_swap64(eiAtomic64 *destination, eiInt64 value)
{
	return _InterlockedExchange64(&destination->v, value);
}

inline eiInt64 ei_atomic_cas64(eiAtomic64 *destination, eiInt64 exchange, eiInt64 comperand)
{
	return _InterlockedCompareExchange64(&destination->v, exchange, comperand);
}

#endif

#else

#include <sched.h>

inline int __bsf(int v)
{
	int r = 0;
	asm ("bsf %1,%0" : "=r"(r) : "r"(v));
	return r;
}

inline int __btc(int v, int i)
{
	int r = 0;
	asm ("btc %1,%0" : "=r"(r) : "r"(i), "0"(v) : "flags");
	return r;
}

inline void ei_yield()
{
	sched_yield();
}

inline eiInt ei_atomic_add(eiAtomic *destination, eiInt value)
{
	return __sync_fetch_and_add(&destination->v, value);
}

inline eiInt ei_atomic_inc(eiAtomic *destination)
{
	return __sync_add_and_fetch(&destination->v, 1);
}

inline eiInt ei_atomic_dec(eiAtomic *destination)
{
	return __sync_sub_and_fetch(&destination->v, 1);
}

inline eiInt ei_atomic_swap(eiAtomic *destination, eiInt value)
{
	return __sync_lock_test_and_set(&destination->v, value);
}

inline eiInt ei_atomic_cas(eiAtomic *destination, eiInt exchange, eiInt comperand)
{
	return __sync_val_compare_and_swap(&destination->v, comperand, exchange);
}

inline eiInt ei_atomic_clear_mask(eiAtomic *destination, eiInt value)
{
	return __sync_fetch_and_and(&destination->v, ~value);
}

inline eiInt ei_atomic_set_mask(eiAtomic *destination, eiInt value)
{
	return __sync_fetch_and_or(&destination->v, value);
}

#ifdef EI_ARCH_X64

inline eiInt64 ei_atomic_add64(eiAtomic64 *destination, eiInt64 value)
{
	return __sync_fetch_and_add(&destination->v, value);
}

inline eiInt64 ei_atomic_inc64(eiAtomic64 *destination)
{
	return __sync_add_and_fetch(&destination->v, 1);
}

inline eiInt64 ei_atomic_dec64(eiAtomic64 *destination)
{
	return __sync_sub_and_fetch(&destination->v, 1);
}

inline eiInt64 ei_atomic_swap64(eiAtomic64 *destination, eiInt64 value)
{
	return __sync_lock_test_and_set(&destination->v, value);
}

inline eiInt64 ei_atomic_cas64(eiAtomic64 *destination, eiInt64 exchange, eiInt64 comperand)
{
	return __sync_val_compare_and_swap(&destination->v, comperand, exchange);
}

#endif

#endif

#define ei_atomic_read(destination) ((destination)->v)
#define ei_atomic_set(destination, value) (((destination)->v) = (value))

inline eiInt ei_atomic_sub(eiAtomic *destination, eiInt value)
{
	return ei_atomic_add(destination, -value);
}

#ifdef EI_ARCH_X64

inline eiInt64 ei_atomic_sub64(eiAtomic64 *destination, eiInt64 value)
{
	return ei_atomic_add64(destination, -value);
}

#endif

inline void ei_pause(eiInt32 delay)
{
	eiInt32 i;

	for (i = 0; i < delay; ++i)
	{
		_mm_pause();
	}
}

#ifdef EI_ARCH_X64

#define eiAtomicptr eiAtomic64

#define ei_atomic_add_ptr ei_atomic_add64
#define ei_atomic_inc_ptr ei_atomic_inc64
#define ei_atomic_dec_ptr ei_atomic_dec64
#define ei_atomic_swap_ptr ei_atomic_swap64
#define ei_atomic_cas_ptr ei_atomic_cas64
#define ei_atomic_sub_ptr ei_atomic_sub64

#else

#define eiAtomicptr eiAtomic

#define ei_atomic_add_ptr ei_atomic_add
#define ei_atomic_inc_ptr ei_atomic_inc
#define ei_atomic_dec_ptr ei_atomic_dec
#define ei_atomic_swap_ptr ei_atomic_swap
#define ei_atomic_cas_ptr ei_atomic_cas
#define ei_atomic_sub_ptr ei_atomic_sub

#endif

#endif
