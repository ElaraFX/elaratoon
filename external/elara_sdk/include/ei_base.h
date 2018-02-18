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

#ifndef EI_BASE_H
#define EI_BASE_H

/** Definitions for multiple platforms.
 * \file ei_base.h
 * \author Elvic Liang
 */

/* OSL interoperability */
#if defined eiCORE_EXPORTS || defined EI_OSL_INTEROP

#include <OSL/oslexec.h>

#endif

/* Architecture */
#ifdef _MSC_VER
	#if defined _M_X64
		#define EI_ARCH_X64
	#elif defined _M_IX86
		#define EI_ARCH_X86
	#endif
#elif defined __GNUC__
	#if defined __x86_64 || defined __amd64
		#define EI_ARCH_X64
	#elif defined __i386
		#define EI_ARCH_X86
	#endif
#endif

/* Operating system and endianness */
#ifdef _WIN32
	#define EI_OS_WINDOWS
	#define EI_OS_LITTLE_ENDIAN
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400 /* Windows 2000 minimum required */
	#endif
#elif defined linux || defined __linux__
	#define EI_OS_LINUX
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define EI_OS_LITTLE_ENDIAN
	#else
		#define EI_OS_BIG_ENDIAN
	#endif
#endif

/* Force inlining */
#ifdef EI_OS_LINUX
	#define __forceinline inline
#endif

/* CRT debugging */
#ifdef _DEBUG
#ifndef _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC
#endif
#endif

/* Statements */
#define STMT(stmt)			do { stmt } while (0);

#ifdef _DEBUG
	#define DEBUG_STMT(stmt)	do { stmt } while (0);
#else
	#define DEBUG_STMT(stmt)
#endif

#endif
