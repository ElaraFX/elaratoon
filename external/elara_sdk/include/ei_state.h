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

#ifndef EI_STATE_H
#define EI_STATE_H

/** Rendering state of current ray.
 * \file ei_state.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_color.h>
#include <ei_vector.h>
#include <ei_vector2.h>
#include <ei_vector4.h>
#include <ei_matrix.h>
#include <ei_matrix2.h>
#include <ei_nodesys.h>

/* Maximum number of user data for describing the intersection */
#define EI_MAX_USER_DATA_SIZE		24
#define EI_RAY_BIAS					(1.0e-4f)

/** Ray types */
#define EI_RAY_NONE				0
#define EI_RAY_GLOSSY			(1 << 0)	/**< scattering is glossy */
#define EI_RAY_DIFFUSE			(1 << 1)	/**< scattering is diffuse */
#define EI_RAY_EYE				(1 << 2)	/**< eye ray */
#define EI_RAY_TRANSPARENT		(1 << 3)	/**< transparent ray */
#define EI_RAY_REFLECT			(1 << 4)	/**< reflection ray */
#define EI_RAY_REFRACT			(1 << 5)	/**< refraction ray */
#define EI_RAY_LIGHT			(1 << 6)	/**< light ray */
#define EI_RAY_SHADOW			(1 << 7)	/**< shadow ray */
#define EI_RAY_ENVIRONMENT		(1 << 8)	/**< ray only into environment */
#define EI_RAY_PROBE			(1 << 9)	/**< probe ray */
#define EI_RAY_SSS				(1 << 10)	/**< sub-surface scattering ray */
#define EI_RAY_DISPLACE			(1 << 11)	/**< displacement during tessellation */
#define EI_RAY_GEOMETRY			(1 << 12)	/**< geometry shader */
#define EI_RAY_OUTPUT			(1 << 13)	/**< output shader */
#define EI_RAY_INIT_SHADER		(1 << 14)	/**< init shader */
#define EI_RAY_EXIT_SHADER		(1 << 15)	/**< exit shader */
#define EI_RAY_VOLUME_SCATTER	(1 << 16)	/**< volume scattering ray */
#define EI_RAY_TEXTURE			(1 << 17)	/**< texture shader */

/* Forward declarations */
typedef struct eiRayTLS					eiRayTLS;
typedef struct eiBaseBucket				eiBaseBucket;
typedef struct eiShaderInstParamTable	eiShaderInstParamTable;
typedef struct eiShaderInstanceTLS		eiShaderInstanceTLS;
typedef struct eiRayObjectInstance		eiRayObjectInstance;
typedef struct eiState					eiState;
typedef struct eiLightSample			eiLightSample;
typedef struct eiLightSampler			eiLightSampler;
typedef struct eiLightInstance			eiLightInstance;
typedef struct eiSampleInfo				eiSampleInfo;

/** The description of shader cache */
struct eiShaderCache {
	eiShaderInstParamTable	*root_param_table;	/**< the parameter table of the root node */
	eiUint					size;				/**< the size of the shader cache in bytes not including 
												     this description, NOT including the sizeof(eiShaderCache) */
	void					*arg;				/**< the custom shader calling arguments */
};

/** The user data of intersections */
typedef union eiUserData {
	eiInt			as_int;
	eiScalar		as_scalar;
	eiIndex			as_index;
} eiUserData;

/** An intersection returned by the ray-tracer.
 */
struct eiIntersection {
	eiTag		hit_inst_tag;	/**< ray-traceable instance tag of the hit */
	eiTag		hit_tessel;		/**< the hit tessellation tag */
	eiIndex		hit_tri;		/**< tessellated triangle index of the hit */
	eiIndex		hit_prim;		/**< source primitive index of the hit */
	eiScalar	hit_t;			/**< hit parametric distance */
	eiVector	bary;			/**< barycentric coordinates of the hit in tessellated triangle */
	eiBool		is_light;		/**< the intersection is a light primitive */
	eiUserData	user_data[ EI_MAX_USER_DATA_SIZE ];	/**< the user data about the hit */
};

/** Reset an intersection.
 */
inline void ei_isect_reset(eiIntersection *isect)
{
	isect->hit_inst_tag = EI_NULL_TAG;
	isect->hit_tessel = EI_NULL_TAG;
	isect->hit_prim = EI_NULL_INDEX;
	isect->hit_tri = EI_NULL_INDEX;
	isect->hit_t = EI_MAX_SCALAR;
	isect->is_light = EI_FALSE;
}

/** A ray to trace.
 */
struct eiRay {
	eiVector			E;						/**< ray origin in internal space when called in surface shader */
	eiVector			dEdx;					/**< the derivative of ray origin along raster x in internal space */
	eiVector			dEdy;					/**< the derivative of ray origin along raster y in internal space */
	eiVector			I;						/**< normalized ray direction in internal space when called in surface shader */
	eiVector			dIdx;					/**< the derivative of ray direction along raster x in internal space */
	eiVector			dIdy;					/**< the derivative of ray direction along raster y in internal space */
	eiVector			org;					/**< ray origin in current intersection test space */
	eiVector			dir;					/**< ray direction in current intersection test space */
	eiScalar			time;
	eiScalar			min_t;
	eiScalar			max_t;
	eiInt				ray_type;
	eiBool				trace_lights;
	eiBool				force_trace_skylights;
	eiBool				force_trace_portals;
	eiBool				force_face_both;
	eiInt				num_isects;
	eiInt				isects_cap;
	eiIntersection		*isects;
	eiIntersection		*isect;
	eiTLS				*tls;
	eiRayTLS			*ray_tls;
	eiBaseBucket		*bucket;
	eiTag				trace_inst_tag;
	eiBool				trace_inst_inclusive;
	eiBool				trace_surfaces;
	eiBool				trace_volumes;
	eiScalar            sss_hit_t;
	eiVector            sss_I;
	/* hybrid path tracer specific variables */
	eiState				*state;
};

inline void ei_ray_init(
	eiRay *ray, 
	eiTLS *tls, 
	eiRayTLS *ray_tls, 
	eiBaseBucket *bucket, 
	eiInt ray_type, 
	eiScalar time, 
	eiBool trace_lights, 
	eiScalar max_dist)
{
	ray->dEdx = 0.0f;
	ray->dEdy = 0.0f;
	ray->dIdx = 0.0f;
	ray->dIdy = 0.0f;
	ray->time = time;
	ray->min_t = EI_RAY_BIAS;
	ray->max_t = max_dist;
	ray->ray_type = ray_type;
	ray->force_face_both = EI_FALSE;
	ray->num_isects = 0;
	ray->isects_cap = 0;
	ray->isects = NULL;
	ray->isect = NULL;
	ray->tls = tls;
	ray->ray_tls = ray_tls;
	ray->bucket = bucket;
	ray->trace_inst_tag = EI_NULL_TAG;
	ray->trace_inst_inclusive = EI_FALSE;
	ray->trace_surfaces = EI_TRUE;
	ray->trace_volumes = EI_TRUE;
	ray->trace_lights = trace_lights;
	ray->force_trace_skylights = EI_FALSE;
	ray->force_trace_portals = EI_FALSE;
	ray->state = NULL;
}

EI_API void ei_ray_exit(eiRay *ray);

/** Add a new intersection to our intersection list.
 */
void ei_ray_add_intersection(eiRay *ray, const eiIntersection *isect);

/** Clear all intersections for re-using a ray.
 */
void ei_ray_clear_intersections(eiRay *ray);

/** Should always be identical to OSL::ShaderGlobals
 */
struct eiShaderGlobals {
	eiVector P, dPdx, dPdy;
	eiVector dPdz;
	eiVector I, dIdx, dIdy;
	eiVector N;
	eiVector Ng;
	float u, dudx, dudy;
	float v, dvdx, dvdy;
	eiVector dPdu, dPdv;
	float time;
	float dtime;
	eiVector dPdtime;
	eiVector Ps, dPsdx, dPsdy;
	void *renderstate;
	void *tracedata;
	void *objdata;
	void *context;
	void *renderer;
	void *object2common;
	void *shader2common;
	void *Ci;
	float surfacearea;
	int raytype;
	int flipHandedness;
	int backfacing;
};

/** The structure holds the current state 
 * variables including ray information and 
 * differential geometry, etc.
 */
struct eiState : public eiShaderGlobals {
	eiIntersection				*isect;					/**< the active intersection this state is bound to */
	eiScalar					ray_bias;				/**< the ray bias for next ray computed from intersection */
	eiInt						hit_flags;				/**< attribute flags of the hit */
	eiBool						light_affects_reflection;	/**< for internal use only */
	eiRayObjectInstance			*hit_inst;				/**< cached instance node for the intersection */
	eiNode						*hit_obj;				/**< cached object node for the intersection */
	eiNode						*hit_mtl;				/**< cached material node for the intersection */
	eiBaseBucket				*bucket;				/**< the current sampling bucket */
	eiTLS						*tls;					/**< TLS associated with this state */
	eiRayTLS					*ray_tls;				/**< ray-tracer TLS associated with this state */
	void						*glob;					/**< the pointer for holding current globals */
	eiByte						*shader_cache;			/**< the current shader cache pointer */
	eiNode						*shader;				/**< the current calling shader instance */
	eiShaderInstanceTLS			*shader_tls;			/**< the thread local storage of current calling shader instance */
	eiSampleInfo				*out;					/**< sampling output contains user-defined output variables */
	eiTag						dag_root;				/**< the output of geometry shader */
	/* hybrid path tracer specific variables */
	eiUint						dimension;				/**< the current integral dimension in ray tree */
	eiUint						AA_seed;				/**< the seed for anti-aliasing */
	eiBool						in_volume;				/**< in execution of a volume shader */
	eiInt						glossy_depth;			/**< the current glossy depth */
	eiInt						diffuse_depth;			/**< the current diffuse or diffuse + glossy depth */
	eiInt						total_depth;			/**< the current total depth */
	eiInt                       refract_depth;          /**< the current refraction depth */
	eiInt						TIR_depth;				/**< ray depth of total internal reflection */
	eiUint						split_factor;			/**< the number of splittings happens so far */
	eiVector					Li;						/**< normalized direction from shading point to light sample in internal space */
	eiScalar					Ldisti;					/**< distance from shading point to light sample in internal space */
	eiScalar					Lpdfi;					/**< light probability density function (pdf) */
	eiColor						Cli;					/**< incoming radiance from the light */
	eiScalar					light_raw_contribution;	/**< raw contribution without shadowing from light */
	eiColor                     path_weight;			/**< accummulated color along path */
};

/** Refresh the state, the renderer may reuse a state as 
 * a new one.
 */
EI_API void ei_flush_cache(eiState *state);

/** Get current options from the state.
 */
EI_API eiNode *ei_state_options(eiState * const state);

/** Get current camera from the state.
 */
EI_API eiNode *ei_state_camera(eiState * const state);

/** Interpolate varying or face-varying primitive variable, 
 * returns true if the primitive varible exists, and was 
 * interpolated successfully, otherwise returns false.
 * \param unique_name The unique name string.
 */
EI_API eiBool ei_get_primvar_int(
	eiState * const state, 
	const char *unique_name, 
	eiInt *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_token(
	eiState * const state, 
	const char *unique_name, 
	eiToken *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_bool(
	eiState * const state, 
	const char *unique_name, 
	eiBool *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_index(
	eiState * const state, 
	const char *unique_name, 
	eiIndex *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_tag(
	eiState * const state, 
	const char *unique_name, 
	eiTag *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_node(
	eiState * const state, 
	const char *unique_name, 
	eiTag *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_array(
	eiState * const state, 
	const char *unique_name, 
	eiTag *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_scalar(
	eiState * const state, 
	const char *unique_name, 
	eiScalar *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_color(
	eiState * const state, 
	const char *unique_name, 
	eiColor *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_vector(
	eiState * const state, 
	const char *unique_name, 
	eiVector *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_point(
	eiState * const state, 
	const char *unique_name, 
	eiPoint *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_normal(
	eiState * const state, 
	const char *unique_name, 
	eiVector *result, 
	eiBool derivatives);
EI_API eiBool ei_get_primvar_matrix(
	eiState * const state, 
	const char *unique_name, 
	eiMatrix *result, 
	eiBool derivatives);

/** Allow users to emit custom trace calls to perform hit testing 
 * (pick), or any other tasks which require ray-tracing.
 * \return The number of traced rays in this callback.
 */
typedef eiUint (*ei_process_trace)(
	eiProcess *process, 
	eiTLS *tls, 
	eiBaseBucket *bucket, 
	eiTag scene_tag);

EI_API void ei_set_custom_trace(ei_process_trace cb);

#endif
