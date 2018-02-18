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

#ifndef EI_RANDOM_H
#define EI_RANDOM_H

#include <ei_util.h>
#include <ei_vector.h>

#define EI_DEFAULT_RANDOM_SEED	0x9e3779b1

inline eiUint ei_hash(eiUint a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

/** A fast random number generator based 
 * on Thomas Wang's hash function.
 */
typedef struct eiFastRandomGen {
	eiUint		state;
} eiFastRandomGen;

inline void ei_fast_random_reset(eiFastRandomGen *gen, const eiUint seed)
{
	gen->state = ei_hash(seed);
}

/** Output 32 random bits */
inline eiUint ei_fast_brandom(eiFastRandomGen *gen)
{
	gen->state = ei_hash(gen->state);
	return gen->state;
}

/** Output random float number in the interval 0 <= x < 1 */
inline eiScalar ei_fast_random(eiFastRandomGen *gen)
{
	/* get 32 random bits and convert to float */
	gen->state = ei_hash(gen->state);
	return (gen->state & 0xFFFFFF) * (1.0f / eiScalar(1 << 24));
}

/** Output random integer in the interval min <= x <= max */
inline eiInt ei_fast_irandom(eiFastRandomGen *gen, const eiInt min, const eiInt max)
{
	eiInt	r;
	
	/* multiply interval with random and truncate */
	r = (eiInt)((eiScalar)(max - min + 1) * ei_fast_random(gen)) + min;
	if (r > max)
	{
		r = max;
	}
	return r;
}

/** Generate a random permuation with n elements.
 */
inline void ei_random_perm(eiUint perm[], eiUint n, eiFastRandomGen *gen)
{
	for (eiUint i = 0; i < n; ++i)
	{
		perm[i] = i;
	}

	for (eiUint i = 0; i < n; ++i)
	{
		eiUint j = ei_fast_irandom(gen, 0, i);
		eiUint temp = perm[i];
		perm[i] = perm[j];
		perm[j] = temp;
	}
}

/** C style replication of std::upper_bound
 */
inline eiScalar *ei_upper_bound(eiScalar *_First, eiUint _Count, eiScalar _Val)
{
	for (; 0 < _Count;)
	{
		eiUint _Count2 = (_Count >> 1);
		eiScalar *_Mid = _First + _Count2;

		if (!(_Val < *_Mid))
		{
			_First = ++_Mid;
			_Count -= _Count2 + 1;
		}
		else
		{
			_Count = _Count2;
		}
	}

	return _First;
}

inline eiScalar ei_sample_cdf(eiScalar *data, eiUint n, const eiScalar x, eiUint & idx, eiScalar & pdf)
{
	idx = (eiUint)(ei_upper_bound(data, n, x) - data);
	idx = clamp<eiUint>(idx, 0, n - 1);
	eiScalar scaled_sample;
	if (idx == 0)
	{
		pdf = data[0];
		scaled_sample = x;
	}
	else
	{
		pdf = data[idx] - data[idx - 1];
		scaled_sample = x - data[idx - 1];
	}
	pdf = max(EI_NORM_EPS, pdf);
	return scaled_sample / pdf;
}

#endif
