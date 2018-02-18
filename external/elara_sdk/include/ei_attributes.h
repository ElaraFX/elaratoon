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
#ifndef EI_ATTRIBUTES_H
#define EI_ATTRIBUTES_H

/** The attributes of instance.
 * \file ei_attributes.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_dataflow.h>
#include <ei_nodesys.h>

/* visible to all ray types */
#define EI_ATTR_VISIBLE					(1 << 0)
/* visible to secondary (reflection or refraction) rays */
#define EI_ATTR_VISIBLE_SECONDARY		(1 << 1)
/* visible to shadow rays (cast shadows) */
#define EI_ATTR_CAST_SHADOW				(1 << 2)
/* whether this object receives shadows */
#define EI_ATTR_RECV_SHADOW				(1 << 3)
/* visible to reflection rays (cast reflection) */
#define EI_ATTR_CAST_REFLECTION			(1 << 4)
/* whether this object receives reflections */
#define EI_ATTR_RECV_REFLECTION			(1 << 5)
/* visible to refraction rays (cast refraction) */
#define EI_ATTR_CAST_REFRACTION			(1 << 6)
/* whether this object receives refractions */
#define EI_ATTR_RECV_REFRACTION			(1 << 7)
/* visible to transparency rays (cast transparency) */
#define EI_ATTR_CAST_TRANSPARENCY		(1 << 8)
/* whether this object receives transparency */
#define EI_ATTR_RECV_TRANSPARENCY		(1 << 9)
/* enable/disable motion blur */
#define EI_ATTR_MOTION_BLUR				(1 << 10)
/* enable/disable displacement mapping */
#define EI_ATTR_DISPLACE				(1 << 11)
/* the object is flagged as opaque, for internal use only */
#define EI_ATTR_OPAQUE					(1 << 12)
/* the object is a lightsource, for internal use only */
#define EI_ATTR_EMISSIVE				(1 << 13)
/* the object instance has negative parity in transform, 
   for internal use only */
#define EI_ATTR_NEG_PARITY				(1 << 14)
/* visible to primary (eye) rays */
#define EI_ATTR_VISIBLE_PRIMARY			(1 << 15)
/* use light group */
#define EI_ATTR_USE_LIGHT_GROUP			(1 << 16)
/* use shadow group */
#define EI_ATTR_USE_SHADOW_GROUP		(1 << 17)
/* surface shader contains emission, for internal use only */
#define EI_ATTR_SURFACE_EMISSION		(1 << 18)
/* is shadow enabled for a light */
#define EI_ATTR_SHADOW					(1 << 19)
/* enable shadow terminator fix */
#define EI_ATTR_SHADOW_TERMINATOR_FIX	(1 << 20)
/* does the volume cast global illumination */
#define EI_ATTR_VOLUME_CAST_GI    		(1 << 21)
/* use multple importance sampling */
#define EI_ATTR_USE_MIS		    		(1 << 22)

/** The attributes of instance.
 */
#pragma pack(push, 1)
struct eiAttributes {
	eiTag					approx;
	eiTag					mtl_list;
	eiTag					light_group;
	eiTag					shadow_group;
	eiInt					flags;			/**< attribute flags */
	eiScalar				max_displace;
	eiScalar				bias;			/**< the ray bias to avoid self-intersection */
	eiScalar				step_size;		/**< step size for volume rendering */
};
#pragma pack(pop)

/** Set attributes to nulls, mainly for light nodes.
 */
void ei_attr_set_nulls(eiAttributes *attr);

/** Set all attributes to default settings, inherit some 
 * attributes from global options.
 */
void ei_attr_set_defaults(eiAttributes *attr, eiNode *opt);

/** Copy the attributes.
 */
void ei_attr_copy(eiAttributes *dst, const eiAttributes *src);

/** Compare two attributes.
 */
eiInt ei_attr_compare(const eiAttributes *attr1, const eiAttributes *attr2);

/** Inherit from parent attributes, if an attribute is by the child (null), 
 * the corresponding value from its parent will be used.
 */
void ei_attr_inherit(eiAttributes *attr, const eiAttributes *parent);

void ei_attr_set_flag(eiAttributes *attr, const eiInt flag, const eiBool value);

inline eiBool ei_attr_get_flag(const eiAttributes *attr, const eiInt flag)
{
	return ((attr->flags & flag) != 0);
}

#endif
