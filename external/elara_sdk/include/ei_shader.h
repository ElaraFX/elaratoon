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

#ifndef EI_SHADER_H
#define EI_SHADER_H

/** Header of shading APIs.
 * for consistency, please do NOT change this file.
 * \file ei_shader.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_nodesys.h>
#include <ei_state.h>

/* Exporting */
#ifdef _MSC_VER
#	define EI_SHADER_API			EI_EXTERN __declspec(dllexport)
#else
#	define EI_SHADER_API			EI_EXTERN __attribute__((visibility("default")))
#endif

typedef eiBool (*ei_shader_main_func)(
	eiState * const state, 
	void *arg);
typedef eiBool (*ei_shader_init_node_func)(
	eiState * const state, 
	void *arg);
typedef eiBool (*ei_shader_exit_node_func)(
	eiState * const state, 
	void *arg);

/** The description of a shader class */
typedef struct eiShader {
	eiNodeObject				base;		/**< the base node object */
	ei_shader_main_func			main;		/**< the main function */
	ei_shader_init_node_func	init_node;	/**< the init_node function */
	ei_shader_exit_node_func	exit_node;	/**< the exit_node function */
	eiUint						size;		/**< the size of shader class in bytes */
} eiShader;

EI_API void ei_shader_init(eiShader *shader, eiBool is_volume);
EI_API void ei_shader_exit(eiShader *shader);
EI_API void ei_shader_init_node(eiNode *node);
EI_API void ei_shader_exit_node(eiNode *node);

/** An output of a shader call */
typedef struct eiShaderOutput {
	eiInt			type;
	const char		*name;
	void			*value;
} eiShaderOutput;

/** Call a shader by tag. the return value of the 
 * shader call will be returned. the shader can be either 
 * one shader or a list of shaders.
 */
EI_API eiBool ei_call_shader(
	eiState * const state, 
	const eiTag shader, 
	const eiUint num_outputs, 
	eiShaderOutput *outputs, 
	void *arg);

/** Evaluate the value of a shader parameter.
 */
EI_API eiInt *ei_eval_int(eiState *state, eiIndex param_index);
EI_API eiToken *ei_eval_token(eiState *state, eiIndex param_index);
EI_API eiBool *ei_eval_bool(eiState *state, eiIndex param_index);
EI_API eiIndex *ei_eval_index(eiState *state, eiIndex param_index);
EI_API eiTag *ei_eval_tag(eiState *state, eiIndex param_index);
EI_API eiTag *ei_eval_node(eiState *state, eiIndex param_index);
EI_API eiTag *ei_eval_array(eiState *state, eiIndex param_index);
EI_API eiScalar *ei_eval_scalar(eiState *state, eiIndex param_index);
EI_API eiColor *ei_eval_color(eiState *state, eiIndex param_index);
EI_API eiVector *ei_eval_vector(eiState *state, eiIndex param_index);
EI_API eiVector *ei_eval_point(eiState *state, eiIndex param_index);
EI_API eiVector *ei_eval_normal(eiState *state, eiIndex param_index);
EI_API eiMatrix *ei_eval_matrix(eiState *state, eiIndex param_index);

/** Get the unique name string of a shader parameter.
 */
EI_API const char *ei_shader_param_unique_name(eiState *state, eiIndex param_index);

/** Compute a deterministic random number from current state.
 */
EI_API eiScalar ei_state_random(eiState * const state);
EI_API void ei_raster_pos(eiVector2 *result, eiState * const state);

/** Transform a point from internal space to camera space.
 */
EI_API void ei_to_camera(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from internal space to camera space.
 */
EI_API void ei_vto_camera(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from internal space to camera space.
 */
EI_API void ei_nto_camera(eiVector * const r, const eiVector *v, eiState * const state);

/** Transform a point from camera space to internal space.
 */
EI_API void ei_from_camera(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from camera space to internal space.
 */
EI_API void ei_vfrom_camera(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from camera space to internal space.
 */
EI_API void ei_nfrom_camera(eiVector * const r, const eiVector *v, eiState * const state);

/** Transform a point from internal space to object space.
 */
EI_API void ei_to_object(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from internal space to object space.
 */
EI_API void ei_vto_object(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from internal space to object space.
 */
EI_API void ei_nto_object(eiVector * const r, const eiVector *v, eiState * const state);

/** Transform a point from object space to internal space.
 */
EI_API void ei_from_object(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from object space to internal space.
 */
EI_API void ei_vfrom_object(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from object space to internal space.
 */
EI_API void ei_nfrom_object(eiVector * const r, const eiVector *v, eiState * const state);

/** Transform a point from internal space to light space.
 */
EI_API void ei_to_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);
/** Transform a vector from internal space to light space.
 */
EI_API void ei_vto_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);
/** Transform a normal from internal space to light space.
 */
EI_API void ei_nto_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);

/** Transform a point from light space to internal space.
 */
EI_API void ei_from_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);
/** Transform a vector from light space to internal space.
 */
EI_API void ei_vfrom_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);
/** Transform a normal from light space to internal space.
 */
EI_API void ei_nfrom_light(eiVector * const r, const eiVector *v, const eiLightSampler *sampler);

/** Transform a point from internal space to world space.
 */
EI_API void ei_to_world(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from internal space to world space.
 */
EI_API void ei_vto_world(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from internal space to world space.
 */
EI_API void ei_nto_world(eiVector * const r, const eiVector *v, eiState * const state);

/** Transform a point from world space to internal space.
 */
EI_API void ei_from_world(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a vector from world space to internal space.
 */
EI_API void ei_vfrom_world(eiVector * const r, const eiVector *v, eiState * const state);
/** Transform a normal from world space to internal space.
 */
EI_API void ei_nfrom_world(eiVector * const r, const eiVector *v, eiState * const state);

/** Set the root node for current procedural object.
 */
EI_API void ei_geometry_root(const char *root_instgroup, eiState * const state);

/* Message readers */
EI_API eiBool ei_msg_get_token(
	eiState * const state, 
	const char *name, 
	eiToken *value);
EI_API eiBool ei_msg_get_int(
	eiState * const state, 
	const char *name, 
	eiInt *value);
EI_API eiBool ei_msg_get_scalar(
	eiState * const state, 
	const char *name, 
	eiScalar *value);
EI_API eiBool ei_msg_get_color(
	eiState * const state, 
	const char *name, 
	eiColor *value);
EI_API eiBool ei_msg_get_vector(
	eiState * const state, 
	const char *name, 
	eiVector *value);
EI_API eiBool ei_msg_get_point(
	eiState * const state, 
	const char *name, 
	eiPoint *value);
EI_API eiBool ei_msg_get_vector4(
	eiState * const state, 
	const char *name, 
	eiVector4 *value);
EI_API eiBool ei_msg_get_tag(
	eiState * const state, 
	const char *name, 
	eiTag *value);
EI_API eiBool ei_msg_get_index(
	eiState * const state, 
	const char *name, 
	eiIndex *value);
EI_API eiBool ei_msg_get_bool(
	eiState * const state, 
	const char *name, 
	eiBool *value);

/* Message writers */
EI_API eiBool ei_msg_set_token(
	eiState * const state, 
	const char *name, 
	const eiToken value);
EI_API eiBool ei_msg_set_int(
	eiState * const state, 
	const char *name, 
	const eiInt value);
EI_API eiBool ei_msg_set_scalar(
	eiState * const state, 
	const char *name, 
	const eiScalar value);
EI_API eiBool ei_msg_set_color(
	eiState * const state, 
	const char *name, 
	const eiColor *value);
EI_API eiBool ei_msg_set_vector(
	eiState * const state, 
	const char *name, 
	const eiVector *value);
EI_API eiBool ei_msg_set_point(
	eiState * const state, 
	const char *name, 
	const eiPoint *value);
EI_API eiBool ei_msg_set_vector4(
	eiState * const state, 
	const char *name, 
	const eiVector4 *value);
EI_API eiBool ei_msg_set_tag(
	eiState * const state, 
	const char *name, 
	const eiTag value);
EI_API eiBool ei_msg_set_index(
	eiState * const state, 
	const char *name, 
	const eiIndex value);
EI_API eiBool ei_msg_set_bool(
	eiState * const state, 
	const char *name, 
	const eiBool value);

/** The camera node */
enum {
	EI_CAMERA_output_list = 0,				/**< a data table of outputs */
	EI_CAMERA_imager_shader, 
	EI_CAMERA_atmo_shader, 
	EI_CAMERA_env_shader, 
	EI_CAMERA_lens_shader, 
	EI_CAMERA_focal, 
	EI_CAMERA_aperture, 
	EI_CAMERA_aspect, 
	EI_CAMERA_res_x, 
	EI_CAMERA_res_y, 
	EI_CAMERA_window_xmin, 
	EI_CAMERA_window_xmax, 
	EI_CAMERA_window_ymin, 
	EI_CAMERA_window_ymax, 
	EI_CAMERA_clip_hither, 
	EI_CAMERA_clip_yon, 
	EI_CAMERA_atmo_steps, 
	/* transforms */
	EI_CAMERA_camera_to_world, 
	EI_CAMERA_motion_camera_to_world, 
	EI_CAMERA_world_to_camera, 
	EI_CAMERA_motion_world_to_camera, 
	/* OSL specific transforms */
	EI_CAMERA_screen_to_world, 
	EI_CAMERA_NDC_to_world, 
	EI_CAMERA_raster_to_world, 
	EI_CAMERA_world_to_screen, 
	EI_CAMERA_world_to_NDC, 
	EI_CAMERA_world_to_raster, 
	/* precomputed parameters */
	EI_CAMERA_image_center_x, 
	EI_CAMERA_image_center_y, 
	EI_CAMERA_camera_to_pixel_x, 
	EI_CAMERA_camera_to_pixel_y, 
	EI_CAMERA_pixel_to_camera_x, 
	EI_CAMERA_pixel_to_camera_y, 
	EI_CAMERA_coeffz, 
	EI_CAMERA_constz, 
	EI_CAMERA_focalz, 
	EI_CAMERA_project_near_clip, 
	EI_CAMERA_world_up, 
	EI_CAMERA_clip_auto_hither, 
	EI_CAMERA_camera_to_world_moving, 
	EI_CAMERA_COUNT, 
};

struct eiCameraOutput
{
	eiVector	E, dEdx, dEdy;
	eiVector	I, dIdx, dIdy;
	eiScalar	t_near, t_far;
	eiColor		Ci;
	eiColor		Oi;
};

enum
{
	EI_FEATURE_VIEWDEP_DISPLACEMENT = 0,	/**< view-dependent displacement mapping */
	EI_FEATURE_MULTI_VIEW_RENDER,			/**< render multiple views in one image */
};

/** Whether the camera supports a specific feature of the renderer
 */
typedef eiBool (*ei_camera_support)(
	eiState * const state, 
	eiNode *cam, 
	eiInt feature, 
	void *feature_params);
/** Try to project position from object space to screen 
 * space.
 * \return True when position is projectable, false if not. 
 * the output position is undefined when false is returned.
 */
typedef eiBool (*ei_camera_object_to_screen)(
	eiState * const state, 
	eiNode *cam, 
	eiVector *rpos, 
	const eiVector *opos, 
	const eiMatrix *object_to_view);
/** World bounding box is updated, ask the camera to do 
 * some relevant updates if applicable.
 */
typedef eiBool (*ei_camera_update_world_bbox)(
	eiState * const state, 
	eiNode *cam, 
	const eiBBox *world_bbox);
/** Ask the camera to generate world space ray, path 
 * differentials and clipping parameters given raster 
 * space coordinates.
 */
typedef eiBool (*ei_camera_generate_ray)(
	eiState * const state, 
	eiNode *cam, 
	eiCameraOutput *out);

/** The description of a camera shader class
 */
struct eiCameraShader
{
	eiShader						base;
	ei_camera_support				support;
	ei_camera_object_to_screen		object_to_screen;
	ei_camera_update_world_bbox		update_world_bbox;
	ei_camera_generate_ray			generate_ray;
};

EI_API void ei_camera_shader_init(eiCameraShader *shader, eiBool is_volume);
EI_API void ei_camera_shader_exit(eiCameraShader *shader);
EI_API void ei_camera_shader_init_node(eiNode *node);
EI_API void ei_camera_shader_exit_node(eiNode *node);

EI_API eiBool ei_std_camera_support(
	eiNode *cam, 
	eiInt feature, 
	void *feature_params);
EI_API eiBool ei_std_camera_object_to_screen(
	eiNode *cam, 
	eiVector *rpos, 
	const eiVector *opos, 
	const eiMatrix *object_to_view);
EI_API eiBool ei_std_camera_update_world_bbox(
	eiNode *cam, 
	const eiBBox *world_bbox);
EI_API eiBool ei_std_camera_generate_ray(
	eiState * const state, 
	eiNode *cam, 
	eiCameraOutput *out);
EI_API void ei_ray_from_camera(
	eiCameraOutput *out, 
	eiNode *cam, 
	eiScalar time);
EI_API void ei_lens_sample(
	eiVector2 *result, 
	eiState * const state);

/** An extra attribute for texture shader
 */
struct eiTextureAttribute
{
	eiToken			name;
	eiInt			type;
	eiParamValue	value;
};

/** Texture shader evaluation parameters
 */
struct eiTextureShaderParams
{
	/* input parameters */
	const char	*filename;

	/* output parameters */
	eiInt		xres;
	eiInt		yres;
	eiInt		channels;
	eiBool		is_float;
	ei_array	extra_attribs;
	ei_array	img_data;
};

/** The default implementation to load images using OpenImageIO
 */
EI_API eiBool ei_oiio_load_image(eiTextureShaderParams *params);

/** Compile an OSL surface shader into GLSL code string. caller 
 * must be responsible for freeing the returned string buffer 
 * using ei_free.
 */
EI_API char *ei_shader_compile(
	eiNode *node, 
	ei_array *textures_needed, 
	ei_array *attributes_needed, 
	ei_array *attribute_scopes, 
	const char *language);

#endif
