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

#ifndef EI_ASSERT_H
#define EI_ASSERT_H

/** Macros for issuing assertion failures with 
 * file name and line number.
 * \file ei_assert.h
 * \author Elvic Liang
 */

#include <ei_verbose.h>

/* CRT debugging */
#ifdef _DEBUG
#	include <crtdbg.h>
#else
#	ifndef _ASSERT
#		define _ASSERT(expr) 
#	endif
#endif

#define EI_ASSERT(expr) if (!(expr)) { ei_error("Assertion failed: %s File: %s Line: %d\n", #expr, __FILE__, __LINE__); _ASSERT(0); }

/* Debug assertion which does not go into release build */
#ifdef _DEBUG
#define EI_DBG_ASSERT(expr) EI_ASSERT(expr)
#else
#define EI_DBG_ASSERT(expr) 
#endif

/* Compile time static assert */
#define EI_STATIC_ASSERT(expr, msg) typedef char Assert_failed_ ## msg [ (expr) ? 1 : -1 ];

#endif
