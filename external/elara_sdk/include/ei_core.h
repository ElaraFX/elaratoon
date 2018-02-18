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

#ifndef EI_CORE_H
#define EI_CORE_H

/** Header of the Core module.
 * \file ei_core.h
 * \author Elvic Liang
 */

#include <ei_types.h>

/** Core data types */
enum {
	EI_TYPE_NONE = 0,			/**< none data type, must be 0, should not be used actually */
	/* Atomic types */
	EI_TYPE_TOKEN,				/**< eiToken */
	EI_TYPE_BYTE,				/**< eiByte */
	EI_TYPE_INT,				/**< eiInt */
	EI_TYPE_ENUM,				/**< enum */
	EI_TYPE_BITFIELDS,			/**< bitfields */
	EI_TYPE_BOOL,				/**< eiBool */
	EI_TYPE_TAG,				/**< eiTag */
	EI_TYPE_TAG_NODE,			/**< tag to a node */
	EI_TYPE_TAG_ARRAY,			/**< tag to an array */
	EI_TYPE_INDEX,				/**< eiIndex */
	EI_TYPE_LONG,				/**< eiLong */
	EI_TYPE_SCALAR,				/**< eiScalar */
	EI_TYPE_GEOSCALAR,			/**< eiGeoScalar */
	EI_TYPE_COLOR,				/**< eiColor */
	EI_TYPE_VECTOR,				/**< eiVector */
	EI_TYPE_POINT,				/**< eiPoint */
	EI_TYPE_VECTOR2,			/**< eiVector2 */
	EI_TYPE_VECTOR4,			/**< eiVector4 */
	EI_TYPE_CVECTOR,			/**< eiCVector */
	EI_TYPE_HVECTOR,			/**< eiHVector */
	EI_TYPE_HVECTOR2,			/**< eiHVector2 */
	EI_TYPE_MATRIX,				/**< eiMatrix */
	EI_TYPE_BBOX,				/**< eiBBox */
	EI_TYPE_RECT,				/**< eiRect */
	EI_TYPE_RECT4I,				/**< eiRect4i */
	EI_TYPE_STRING,				/**< eiByte array (string) */
	EI_TYPE_CLOSURE_COLOR,		/**< closure color */
	/* Compound types */
	EI_TYPE_TABLE,				/**< dynamic data table stored in database */
	EI_TYPE_USER,				/**< user data type starts from here */
};

/** TLS interfaces */
enum {
	EI_TLS_TYPE_NONE = 0,		/**< none TLS type, must be 0, should not be used */
	EI_TLS_TYPE_USER,			/**< user TLS type starts from here */
};

/* Extern "C" */
#ifdef __cplusplus
#define EI_EXTERN extern "C"
#else
#define EI_EXTERN extern
#endif

/* Exporting and importing */
/* eiCORE_EXPORTS must NOT be defined by users */
#ifdef _MSC_VER
#	if defined eiCORE_EXPORTS
#		define EI_XAPI		__declspec(dllexport)
#	else
#		define EI_XAPI		__declspec(dllimport)
#	endif
#else
#	define EI_XAPI			__attribute__((visibility("default")))
#endif

#define EI_API EI_EXTERN EI_XAPI

/** Convert type constant to name string.
 */
EI_API const char *ei_type_name_string(int type);

#endif
