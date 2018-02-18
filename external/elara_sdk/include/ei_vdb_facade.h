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

#ifndef EI_VDB_FACADE_H
#define EI_VDB_FACADE_H

#ifndef EI_API

/* extern "C" */
#ifdef __cplusplus
#define EI_EXTERN extern "C"
#else
#define EI_EXTERN extern
#endif

/* exporting and importing */
/* eiCORE_EXPORTS should NOT be defined by users */
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

#endif

struct eiVDBPoint
{
	inline eiVDBPoint() : 
		x(0.0), 
		y(0.0), 
		z(0.0)
	{
	}

	inline eiVDBPoint(double _x, double _y, double _z) : 
		x(_x), 
		y(_y), 
		z(_z)
	{
	}

	double x;
	double y;
	double z;
};

/** Interface class for GPU rendering of an object.
 */
class eiRenderObject
{
public:
	eiRenderObject() {}
	virtual ~eiRenderObject() {}

	virtual void DeleteThis() = 0;

	virtual float *getVertexBuffer() const = 0;
	virtual float *getColorBuffer() const = 0;
	virtual unsigned int *getIndexBuffer() const = 0;
	virtual unsigned int getBufferSize() const = 0;

	virtual void getBBoxCorners(eiVDBPoint *corners) const = 0;
};

/** Create a render object from a specific grid in a VDB file.
 * User must call DeleteThis on the returned object once it's 
 * no longer in use.
 */
EI_API eiRenderObject *ei_create_vdb_render_object(
	const char *filename, 
	const char *gridname);

/** Get the grid name list from a VDB file for users to select later.
 */
typedef void (*eiGridNameCallback)(const char *grid_name, void *param);

EI_API void ei_get_grid_names(const char *filename, eiGridNameCallback cb, void *param);

#endif
