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

#ifndef EI_RAYTRACER_H
#define EI_RAYTRACER_H

/** Low-level APIs of ray-tracing core.
 * \file ei_raytracer.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_attributes.h>
#include <ei_state.h>
#include <ei_vector.h>
#include <ei_vector4.h>
#include <ei_matrix.h>
#include <ei_platform.h>
#include <ei_array.h>
#include <ei_slist.h>

/* Forward declarations */
typedef struct eiRaySubscene		eiRaySubscene;
typedef struct eiRayScene			eiRayScene;
typedef struct eiRayTessel			eiRayTessel;

#pragma pack(push, 1)

/** The ray-tracing camera settings.
 * this data should be provided by users.
 */
struct eiRayCamera {
	eiBool			camera_to_world_moving;	/**< for internal use only */
	eiMatrix		camera_to_world;		/**< provided by users */
	eiMatrix		motion_camera_to_world;	/**< provided by users */
	eiMatrix		world_to_camera;		/**< for internal use only */
	eiMatrix		motion_world_to_camera;	/**< for internal use only */
};

/** The ray-tracing options.
 * this data should be provided by users.
 */
struct eiRayOptions {
	eiInt		accel_mode;
};

/** The triangle for geometry pipeline.
 */
struct eiGeoTriangle {
	eiIndex		v1;
	eiIndex		v2;
	eiIndex		v3;
	eiIndex		src_prim_index;
};

/** The tessellated ray-traceable triangle.
 */
struct eiRayTriangle {
	eiIndex		flags;		/**<  1 bit: deform moving; 
							     31 bits: source primitive index */
};

#define RAY_TRI_PRIM_INDEX(tri)							((eiIndex)((tri)->flags & 0x7FFFFFFF))
#define RAY_TRI_DEFORM_MOVING(tri)						(((tri)->flags & 0x80000000) != 0)
#define RAY_TRI_SET_PRIM_INDEX(tri, prim_index)			(tri)->flags = (prim_index & 0x7FFFFFFF);
#define RAY_TRI_SET_DEFORM_MOVING(tri, deform_moving)	(tri)->flags |= ((deform_moving) ? 0x80000000 : 0);

/** The tessellation type */
enum {
	EI_TESSEL_NONE = 0, 
	EI_TESSEL_STATIC,		/**< predefined tessellation */
	EI_TESSEL_DEFERRED,		/**< deferred tessellation will generate tessellated 
							     micro-triangles at render-time */
	EI_TESSEL_PROCEDURAL,	/**< procedural tessellation will create sub-scene 
							     at render-time */
	EI_TESSEL_TYPE_COUNT, 
};

/** One face-varying channel entry in tessellation.
 */
struct eiFVarChannel {
	eiTag			values;		/**< data table of values */
	eiTag			indices;	/**< data table of indices */
};

/** The ray-traceable tessellation of a small portion of an 
 * object. this is the class we will use to interoperate with 
 * geometry approximation module.
 */
struct eiRayTessel {
	eiTag			tag;					/**< the tag of this tessellation in database */
	eiTag			job;					/**< when type is EI_RAY_TESSEL_STATIC, the tessellation is 
											     predefined and already tessellated into micro-triangles; 
											     when type is EI_RAY_TESSEL_DEFERRED, the job tag refers 
											     to a node whose dice function is to be called when the 
											     bounding box of the tessellation is hit by a ray, the dice 
											     function should use the high-level custom geometry data 
											     stored in this tessellation to generated micro-triangles; 
											     when type is EI_RAY_TESSEL_PROCEDURAL, the job tag refers 
											     to a list of geometry shaders which is to be called when 
											     the bounding box of the tessellation is hit by a ray, 
											     the geometry shaders should create a sub-scene, the root 
											     of the sub-scene must be an instance group attached to 
											     this tessellation, for life-time management, the sub-scene 
											     will be deleted when this tessellation is deleted since 
											     it's created by this tessellation. */
	eiInt			type;					/**< the tessellation type */
	eiUint			num_keys;				/**< the number of deformed motion keys */
	/* tessellated ray-traceable vertices and triangles */
	eiUint			num_vertices;
	eiUint			num_triangles;
	eiUint			triangle_count;
	eiUint			num_vertex_channels;	/**< the number of user-defined vertex channels */
	eiTag			pos_list;				/**< the vertex position list */
	eiTag			tri_list;				/**< the vertex index (triangle) list */
	eiTag			tree;					/**< the tree for primitives in this tessellation */
	eiTag			scene;					/**< the ray-traceable scene */
	eiTag			subscene_tag;			/**< for procedural tessellation only, the ray-traceable 
											     sub-scene generated by procedural object */
	eiBBox			box;					/**< the box enclosing all vertices (no displacement) */
	eiBBox			motion_box;				/**< the box enclosing all motion vertices (no displacement) */
	eiUint			vertex_channel_offset;	/**< the offset of user-defined vertex channels in bytes */
	eiInt			normal_channel_type;	/**< the data type of normal channel */
	eiTag			normal_channel_tag;		/**< the tag of normal channel */
	eiTag			normal_channel_indices;	/**< the index list when normal channel is face-varying */
	eiUint			num_fvar_channels;		/**< the number of face-varying channels */
	eiUint			fvar_channel_offset;	/**< the offset of user-defined face-varying channels in bytes */
};

/** Begin describing a tessellation, pre-allocate 
 * memory for vertex list and triangle list. this function 
 * will create a new tessellation in database.
 */
EI_API eiRayTessel *ei_rt_tessel(
	eiTag * const tag, 
	const eiBool motion, 
	const eiUint num_vertices, 
	const eiUint num_triangles, 
	const eiUint num_vertex_channels, 
	const eiUint num_fvar_channels, 
	const eiTag scene);

/** Set data for the position channel.
 * It must contains the same number of motion keys specified 
 * in the declaration of this tessellation.
 */
EI_API void ei_rt_tessel_set_position_channel(
	eiRayTessel *tessel, 
	const eiTag tag);

/** Set data for the vertex index (triangle) channel.
 */
EI_API void ei_rt_tessel_set_triangle_channel(
	eiRayTessel *tessel, 
	const eiTag tag);

/** Set data for a user-defined vertex channel.
 */
EI_API void ei_rt_tessel_set_vertex_channel(
	eiRayTessel *tessel, 
	const eiIndex index, 
	const eiTag tag);

/** Set data for a user-defined face-varying channel.
 */
void ei_rt_tessel_set_fvar_channel(
	eiRayTessel *tessel, 
	const eiIndex index, 
	const eiTag tag, 
	const eiTag idx_tag);

/** Flag a vertex channel as normal channel. This channel 
 * must have already been set with ei_rt_tessel_set_vertex_channel 
 * or ei_rt_tessel_set_fvar_channel
 */
EI_API void ei_rt_tessel_set_normal_channel(
	eiRayTessel *tessel, 
	const eiTag tag);

/** Get the number of motion keys in this tessellation.
 */
inline eiUint ei_rt_tessel_get_num_keys(eiRayTessel *tessel)
{
	return tessel->num_keys;
}

/** Get the number of vertices in this tessellation.
 */
inline eiUint ei_rt_tessel_get_num_vertices(eiRayTessel *tessel)
{
	return tessel->num_vertices;
}

/** Get data of the position channel.
 */
inline eiTag ei_rt_tessel_get_position_channel(eiRayTessel *tessel)
{
	return tessel->pos_list;
}

/** Get data of the vertex index (triangle) channel.
 */
inline eiTag ei_rt_tessel_get_triangle_channel(eiRayTessel *tessel)
{
	return tessel->tri_list;
}

/** Get data of a user-defined vertex channel.
 */
inline eiTag ei_rt_tessel_get_vertex_channel(
	eiRayTessel *tessel, 
	const eiIndex index)
{
	return ((eiTag *)(((eiByte *)tessel) + tessel->vertex_channel_offset))[index];
}

/** Get data of a user-defined face-varying channel.
 */
inline void ei_rt_tessel_get_fvar_channel(
	eiRayTessel *tessel, 
	const eiIndex index, 
	eiTag & tag, 
	eiTag & idx_tag)
{
	eiFVarChannel *fvar = ((eiFVarChannel *)(((eiByte *)tessel) + tessel->fvar_channel_offset)) + index;
	tag = fvar->values;
	idx_tag = fvar->indices;
}

/** Get data type of normal channel.
 */
inline eiInt ei_rt_tessel_get_normal_type(eiRayTessel *tessel)
{
	return tessel->normal_channel_type;
}

/** Get indices of normal channel when it's face-varying.
 */
inline eiTag ei_rt_tessel_get_normal_indices(eiRayTessel *tessel)
{
	return tessel->normal_channel_indices;
}

/** Get data of normal channel.
 */
inline eiTag ei_rt_tessel_get_normal_channel(eiRayTessel *tessel)
{
	return tessel->normal_channel_tag;
}

/** Add a new triangle to a tessellation.
 */
EI_API void ei_rt_tessel_add_triangle(
	eiRayTessel *tessel, 
	const eiGeoTriangle *tri);

/** Get the number of triangles in this tessellation.
 */
inline eiUint ei_rt_tessel_get_num_triangles(eiRayTessel *tessel)
{
	return tessel->num_triangles;
}

/** Get a triangle by index from a tessellation.
 */
inline eiRayTriangle *ei_rt_tessel_get_triangles(eiRayTessel *tessel)
{
	return (eiRayTriangle *)(tessel + 1);
}

/** Get ray-traceable scene.
 */
EI_API eiTag ei_rt_tessel_get_scene(eiRayTessel *tessel);

/** Set ray-traceable sub-scene.
 */
EI_API void ei_rt_tessel_set_subscene(
	eiRayTessel *tessel, 
	const eiTag subscene_tag);

/** Get ray-traceable sub-scene.
 */
EI_API eiTag ei_rt_tessel_get_subscene(eiRayTessel *tessel);

/** End describing a tessellation. This function 
 * finishes the creation of the new tessellation.
 */
EI_API void ei_rt_end_tessel(eiRayTessel *tessel);

/** Specify bounding box for deferred/procedural tessellation.
 */
EI_API void ei_rt_tessel_box(
	eiRayTessel *tessel, 
	const eiBBox *box);
/** Specify motion bounding box for deferred/procedural tessellation.
 */
EI_API void ei_rt_tessel_motion_box(
	eiRayTessel *tessel, 
	const eiBBox *box);

/** Begin describing a deferred tessellation.
 */
EI_API eiRayTessel *ei_rt_defer_tessel(
	eiTag * const tag, 
	const eiTag scene);

/** For a deferred tessellation, it specifies the node 
 * which has a dice function to tessellate the high-level geometry 
 * data stored in this tesssellation into micro-triangles.
 * for a procedural tessellation, it specifies a geometry shader 
 * which is used to create a sub-scene.
 */
EI_API void ei_rt_tessel_job(
	eiRayTessel *tessel, 
	const eiTag job);

/** Resize a tessellation for vertex list and triangle list.
 */
EI_API eiRayTessel *ei_rt_tessel_resize(
	const eiTag tag, 
	const eiBool motion, 
	const eiUint num_vertices, 
	const eiUint num_triangles, 
	const eiUint num_vertex_channels, 
	const eiUint num_fvar_channels);

/** Begin describing a procedural tessellation.
 */
EI_API eiRayTessel *ei_rt_proc_tessel(
	eiTag * const tag, 
	const eiTag scene);

/** The tessellated ray-traceable object 
 * which consists of ray-traceable tessellations.
 */
struct eiRayObject {
	eiTag			source;		/**< the source object */
	eiTag			tessels;	/**< a data array of tags, each tag points to an eiRayTessel */
	eiBool			is_light;	/**< the object is a light source */
	eiUint			padding;
};

/** Initialize a ray-traceable object.
 */
EI_API void ei_ray_object_init(
	eiRayObject *object, 
	const eiTag source, 
	const eiBool is_light);
/** Cleanup a ray-traceable object.
 */
EI_API void ei_ray_object_exit(
	eiRayObject *object);

/** An instance of a ray-traceable object
 */
struct eiRayObjectInstance {
	eiTag			leaf_instance;						/**< the leaf node of the DAG path which generates this instance */
	eiTag			object;								/**< the tag of an eiRayObject */
	eiAttributes	attr;								/**< the merged instance attributes */
	eiByte			object_to_world_ident;				/**< object to world matrix is identity */
	eiByte			object_to_world_moving;
	eiByte			flatten_object_to_world_moving;
	eiByte			padding0;
	eiMatrix		object_to_world;					/**< transforms between object space and parent procedural node */
	eiMatrix		motion_object_to_world;
	eiMatrix		flatten_object_to_world;			/**< flatten transforms between object space and global procedural root */
	eiMatrix		flatten_world_to_object;
	eiMatrix		flatten_motion_object_to_world;
	eiMatrix		flatten_motion_world_to_object;
	const eiLightInstance	*light_inst;				/**< the light instance if this is a light */
	eiByte			is_volume_object;					/**< is this a volume object */
	eiByte			has_volume_shaders;					/**< has this instance been assigned with volume shaders */
	eiByte			is_procedural_object;				/**< is this a procedural object */
	eiByte			procedural_generate_volumes;		/**< can procedural object generate volume objects */
	eiColor			mesh_light_color;					/**< multiply of intensity and color if this object is emissive (mesh light) */
};

#pragma pack(pop)

/** Initialize a ray-traceable object instance.
 */
EI_API void ei_ray_object_instance_init(
	eiRayObjectInstance *instance, 
	const eiTag leaf_instance, 
	const eiTag object, 
	const eiAttributes *attr, 
	const eiMatrix *object_to_world, 
	const eiMatrix *motion_object_to_world, 
	const eiMatrix *procedural_to_world, 
	const eiMatrix *motion_procedural_to_world, 
	const eiLightInstance *light_inst, 
	const eiBool is_volume_object, 
	const eiBool is_procedural_object, 
	const eiBool procedural_generate_volumes);
/** Cleanup a ray-traceable object instance.
 */
EI_API void ei_ray_object_instance_exit(eiRayObjectInstance *instance);

/** Begin describing/editing the scene. Return the tag of created 
 * ray-tracebale scene.
 */
EI_API eiTag ei_rt_scene();

/** Update the ray-tracing camera.
 */
EI_API void ei_rt_camera(const eiTag scene_tag, const eiRayCamera *cam);

/** Update the ray-tracing options.
 */
EI_API void ei_rt_options(const eiTag scene_tag, const eiRayOptions *opt);

/** Get the ray-traceable camera.
 */
EI_API eiRayCamera *ei_rt_get_camera(eiRayScene *scene);

/** Get the ray-traceable options.
 */
EI_API eiRayOptions *ei_rt_get_options(eiRayScene *scene);

/** Get the scene root.
 */
EI_API eiTag ei_rt_scene_root(const eiTag scene_tag);

/** Add an existing tessellation to an object. this function 
 * is thread-safe, so each geometry approximation thread can build 
 * tessellation on its own, and add the tessellation to the scene 
 * concurrently.
 * \param object The tag of the ray-traceable object.
 * \param tessel The tag of the ray-traceable tessellation 
 * to be added.
 */
EI_API void ei_rt_add_tessel(
	const eiTag object, 
	const eiTag tessel);

/** Clear all tessellations attached to an object, this 
 * is called when the object is changed, and re-tessellation is 
 * needed for the object.
 * \param object The tag of the ray-traceable object.
 */
EI_API void ei_rt_clear_tessels(const eiTag object);

/** Set the ray-traceable object instances for a specfic sub-scene.
 * \param scene_tag The specific sub-scene which we will add the object 
 * instances into. 
 * \param object_instances The data array of object instances.
 * \param environment light instances.
 */
EI_API void ei_rt_set_instances(
	const eiTag scene_tag, 
	const eiTag object_instances, 
	const eiTag envlight_insts);

/** Ask all objects instances to unreference all objects for a 
 * specific sub-scene.
 * \param scene_tag The specific sub-scene to remove object references.
 */
EI_API void ei_rt_instances_unref_objects(const eiTag scene_tag);

/** Remove all ray-traceable object instances from a specific sub-scene.
 * \param scene_tag The specific sub-scene which we will remove the object 
 * instances from.
 */
EI_API void ei_rt_remove_instances(const eiTag scene_tag);

/** Remove all ray-traceable objects which are lights from a specific sub-scene.
 */
EI_API void ei_rt_remove_light_objects(const eiTag scene_tag);

/** Begin ray-tracing mode, do pre-processing.
 */
EI_API void ei_rt_begin_trace(const eiTag scene_tag);

/** In tessellation mode, only update scene bounding box, don't build accel.
 */
EI_API void ei_rt_update_scene_box(const eiTag scene_tag);

/** Get the bounding box of the entire scene in world space.
 */
EI_API void ei_rt_scene_box(const eiTag scene_tag, eiBBox * const box);
/** Get the length of scene diagonal in world space.
 */
EI_API eiScalar ei_rt_scene_diag(const eiTag scene_tag);

/** Trace with a specific sub-scene and tree.
 */
EI_API eiBool ei_rt_subscene_trace(
	const eiTag subscene_tag, 
	eiRay *ray);
/** Trace a generic ray within maximum distance specified by ray->max_t.
 * This function will find the nearest intersection if ray->isect is NOT 
 * NULL; If ray->isect is NULL, it will append all intersections into 
 * ray->isects array.
 * \return Whether a valid hit within maximum distance is found.
 */
EI_API eiBool ei_rt_trace(
	const eiTag subscene_tag, 
	eiRay *ray);

/** Compute details including differential geometry for 
 * an intersection with a specific sub-scene.
 */
EI_API void ei_rt_post_trace(eiState *state, const eiRay *ray);

/** A specialized post trace for shadow rays.
 */
EI_API eiBool ei_rt_post_trace_shadow(eiState *state, const eiRay *ray);

/** End ray-tracing mode, do post-processing.
 */
EI_API void ei_rt_end_trace(const eiTag scene_tag, const eiInt milliseconds);

/** End describing/editing the scene.
 */
EI_API void ei_rt_end_scene(const eiTag scene_tag);

#endif
