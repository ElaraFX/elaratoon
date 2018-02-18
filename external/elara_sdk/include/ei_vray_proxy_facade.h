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

#ifndef EI_VRAY_PROXY_FACADE_H
#define EI_VRAY_PROXY_FACADE_H

#include <ei_types.h>
#include <ei_bbox.h>

class eiVRayProxyInterface
{
public:
	virtual eiBBox getBBox() = 0;
	virtual void DeleteThis() = 0;
};

EI_API eiBool ei_parse_vrmesh(const char *filename, float scale, const char *groupname);
EI_API eiVRayProxyInterface *ei_create_vray_proxy(const char *filename);

#endif
