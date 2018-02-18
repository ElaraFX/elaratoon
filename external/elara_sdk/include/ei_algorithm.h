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

#ifndef EI_ALGORITHM_H
#define EI_ALGORITHM_H

/** Routines for common algorithms.
 * \file ei_algorithm.h 
 * \author Elvic Liang
 */

#include <ei_core.h>

/** In-place heap sort routine for array.
 * The compare function should return value == 0 if lhs == rhs, 
 * return value < 0 if lhs < rhs, 
 * return value > 0 if lhs > rhs.
 */
#define EI_DEFINE_HEAPSORT(ClassName, CompareFuncName) \
\
inline void adjust_heap_##ClassName(ClassName *a, const eiIntptr i, const eiIntptr n)\
{\
	ClassName item;\
	eiIntptr j = 2 * i + 1;\
	item = a[i];\
\
	while (j < n)\
	{\
		if (j < (n - 1) && CompareFuncName(&a[j], &a[j + 1]) < 0) /* a[j] < a[j + 1] */\
		{\
			++j;\
		}\
\
		if (CompareFuncName(&item, &a[j]) >= 0) /* item >= a[j] */\
		{\
			break;\
		}\
\
		a[(j - 1) / 2] = a[j];\
		j = 2 * j + 1;\
	}\
\
	a[(j - 1) / 2] = item;\
}\
\
inline void make_heap_##ClassName(ClassName *a, const eiIntptr n)\
{\
	eiIntptr i;\
\
	for (i = ((n - 2) / 2); i >= 0; --i)\
	{\
		adjust_heap_##ClassName(a, i, n);\
	}\
}\
\
inline void ei_heapsort_##ClassName(ClassName *a, const eiIntptr n)\
{\
	ClassName temp;\
	eiIntptr i;\
\
	if (n <= 1)\
	{\
		return;\
	}\
\
	make_heap_##ClassName(a, n);\
\
	for (i = n - 1; i > 0; --i)\
	{\
		temp = a[i];\
		a[i] = a[0];\
		a[0] = temp;\
\
		adjust_heap_##ClassName(a, 0, i);\
	}\
}\
\
inline void ei_partial_heapsort_##ClassName(\
	ClassName *a1, const eiIntptr n1, \
	ClassName *a2, const eiIntptr n2)\
{\
	eiIntptr min_n;\
	eiIntptr i;\
\
	if (n2 < 1)\
	{\
		return;\
	}\
\
	min_n = MIN(n1, n2);\
	for (i = 0; i < min_n; ++i)\
	{\
		a2[i] = a1[i];\
	}\
\
	make_heap_##ClassName(a2, min_n);\
\
	for (i = min_n; i < n1; ++i)\
	{\
		if (CompareFuncName(&a1[i], &a2[0]) < 0) /* a1[i] < a2[0] */\
		{\
			a2[0] = a1[i];\
			adjust_heap_##ClassName(a2, 0, min_n);\
		}\
	}\
\
	ei_heapsort_##ClassName(a2, min_n);\
}

/** Binary search an item in sorted array.
 * \return The index of the item in sorted array, 
 * -1 indicates that the item does not exist.
 */
#define EI_DEFINE_BINSEARCH(ClassName, CompareFuncName) \
\
inline eiIntptr ei_binsearch_##ClassName(ClassName *a, const eiIntptr n, ClassName *x)\
{\
	eiIntptr low = 0, high = n, mid = 0;\
\
	if (n <= 0)\
	{\
		return -1;\
	}\
\
	while (low < (high - 1))\
	{\
		mid = (low + high) / 2;\
		\
		if (CompareFuncName(x, &a[mid]) < 0)\
		{\
			high = mid;\
		}\
		else\
		{\
			low = mid;\
		}\
	}\
\
	if (CompareFuncName(x, &a[low]) == 0)\
	{\
		return low;\
	}\
	else\
	{\
		return -1;\
	}\
}

#endif
