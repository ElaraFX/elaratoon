/**************************************************************************
 * Copyright (C) 2018 Rendease Co., Ltd.
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

#ifndef EI_COMMON_H
#define EI_COMMON_H

/** The common header for all modules based on Core module.
 * \file ei_common.h
 * \author Elvic Liang
 */

#include <ei_core.h>

/* Version */
#define EI_VERSION_MAJOR 1
#define EI_VERSION_MINOR 9
#define EI_VERSION_PATCH 26

#define EI_VERSION (10000 * EI_VERSION_MAJOR + \
                      100 * EI_VERSION_MINOR + \
                            EI_VERSION_PATCH)

#define EI_MAKE_VERSION_STRING2(a, b, c) #a "." #b "." #c
#define EI_MAKE_VERSION_STRING(a, b, c) EI_MAKE_VERSION_STRING2(a, b, c)

#define EI_VERSION_STRING EI_MAKE_VERSION_STRING(EI_VERSION_MAJOR, EI_VERSION_MINOR, EI_VERSION_PATCH)

/* Global configurations */
/* this length must be as small as possible, don't make it 
   bigger than a cache line, because accessing a node can 
   be expensive in that case due to cache miss */
#define EI_MAX_NODE_NAME_LEN			128		/**< deprecated, NOT to use */
/* use a small value to save memory for nodes */
#define EI_MAX_PARAM_NAME_LEN			64

/** Custom data types */
enum {
	/* Ray-tracing module */
	EI_TYPE_RAY_NODE = EI_TYPE_USER,				/**< ray-traceable tree node */
	EI_TYPE_RAY_NODE_UNCOMPRESSED,					/**< ray-traceable uncompressed node */
	EI_TYPE_RAY_FAT_INST,							/**< ray-traceable fat instance */
	EI_TYPE_RAY_SUBSCENE,							/**< ray-traceable sub-scene */
	EI_TYPE_RAY_SCENE,								/**< ray-traceable scene */
	EI_TYPE_RAY_OBJECT,								/**< ray-traceable object */
	EI_TYPE_RAY_OBJECT_INST,						/**< ray-traceable object instance */
	EI_TYPE_RAY_TESSEL,								/**< ray-traceable tessellation */
	EI_TYPE_RAY_ACCEL_TRIANGLES,					/**< ray-traceable accelerated triangles */
	EI_TYPE_RAY_FAT_TREE,							/**< ray-traceable fat tree */
	EI_TYPE_RAY_SUBTREE,							/**< ray-traceable sub-tree */
	EI_TYPE_RAY_POLY_TREE,							/**< ray-traceable poly tree */
	EI_TYPE_RAY_HAIR_TREE,							/**< ray-traceable hair tree */
	EI_TYPE_RAY_PARTI_TREE,							/**< ray-traceable particle tree */
	EI_TYPE_VDB_DATA,								/**< VDB data for volume object */
	EI_TYPE_PROC_SCENE,								/**< procedural sub-scene DAG */
	/* Image module */
	EI_TYPE_BUFFER,									/**< dynamic data buffer stored in database */
	EI_TYPE_FRAMEBUFFER_TILE,						/**< a tile in frame buffer */
	EI_TYPE_FRAMEBUFFER,							/**< frame buffer */
	/* Node system */
	EI_TYPE_NODE_DESC,								/**< node description */
	EI_TYPE_NODE_PARAM_DESC,						/**< node parameter description */
	EI_TYPE_NODE,									/**< node */
	EI_TYPE_NODE_PARAM,								/**< node parameter */
	/* Shading system */
	EI_TYPE_SHADER_INST_PARAM_TABLE,				/**< shader instance parameter table */
	/* Scene manager */
	EI_TYPE_LIGHT_INST,								/**< light instance */
	EI_TYPE_PRIMVAR,								/**< primitive variable */
	EI_TYPE_OBJECT_REPS,							/**< object representations */
	/* Geometry approximation */
	EI_TYPE_POLY_TESSEL,							/**< polygon tessellation */
	EI_TYPE_HAIR_TESSEL,							/**< hair tessellation */
	EI_TYPE_PARTI_TESSEL,							/**< particle tessellation */
	EI_TYPE_VOLUME_TESSEL,							/**< volume tessellation */
	EI_TYPE_PROC_TESSEL,							/**< procedural tessellation */
	/* Renderer */
	EI_TYPE_PIXEL_INFO,								/**< per-pixel info */
	EI_TYPE_JOB_BUCKET,								/**< bucket rendering job */
	EI_TYPE_JOB_TESSEL,								/**< tessellation job */
	EI_TYPE_JOB_PROC_GEN,							/**< procedural generation job */
	EI_TYPE_COUNT, 
};

/** Custom TLS interfaces */
enum {
	EI_TLS_TYPE_RAYTRACER = EI_TLS_TYPE_USER,	/**< ray-tracer TLS interface */
	EI_TLS_TYPE_CONTEXT,						/**< scene parser context */
	EI_TLS_TYPE_NAMESPACE,						/**< the current namespace */
	EI_TLS_TYPE_COUNT, 
};

#endif
