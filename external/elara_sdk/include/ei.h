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

#ifndef EI_H
#define EI_H

/** Header of scene description and shading APIs.
 * for consistency, please do NOT modify this file.
 * \file ei.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_verbose.h>
#include <ei_shader.h>

/** The filter types.
 */
enum {
	EI_FILTER_NONE = 0, 
	EI_FILTER_BOX, 
	EI_FILTER_TRIANGLE, 
	EI_FILTER_CATMULLROM, 
	EI_FILTER_GAUSSIAN, 
	EI_FILTER_SINC, 
	EI_FILTER_COUNT, 
};

/** The back-face culling modes.
 */
enum {
	EI_FACE_NONE = 0, 
	EI_FACE_FRONT, 
	EI_FACE_BACK, 
	EI_FACE_BOTH, 
	EI_FACE_COUNT, 
};

/** The engines for light integrator.
 */
enum {
	EI_ENGINE_PATH_TRACER = 0,				/**< obsolete engine */
	EI_ENGINE_BIDIRECTIONAL_PATH_TRACER,	/**< obsolete engine */
	EI_ENGINE_HYBRID_PATH_TRACER, 
	EI_ENGINE_GI_CACHE, 
	EI_ENGINE_TYPE_COUNT, 
};

/** The GI cache modes.
 */
enum {
	EI_GI_CACHE_REBUILD = 0, 
	EI_GI_CACHE_BUILD_ONLY, 
	EI_GI_CACHE_REUSE, 
	EI_GI_CACHE_TYPE_COUNT, 
};

/** The GI cache preview modes.
 */
enum {
	EI_GI_CACHE_PREVIEW_OFF = 0, 
	EI_GI_CACHE_PREVIEW_FAST = 1, 
	EI_GI_CACHE_PREVIEW_ACCURATE = 2, 
};

/** The accelerator modes.
 */
enum {
	EI_ACCEL_REGULAR = 0, 
	EI_ACCEL_LARGE, 
	EI_ACCEL_TYPE_COUNT, 
};

/** The basis types.
 */
enum {
	EI_BASIS_NONE = 0, 
	EI_BASIS_LINEAR, 
	EI_BASIS_QUADRATIC, 
	EI_BASIS_CUBIC, 
	EI_BASIS_CATMULLROM, 
	EI_BASIS_COUNT, 
};

/** The curve modes.
 */
enum {
	EI_CURVE_NONE = 0, 
	EI_CURVE_RIBBON, 
	EI_CURVE_THICK, 
	EI_CURVE_COUNT, 
};

/** The particle modes.
 */
enum {
	EI_PARTI_NONE = 0, 
	EI_PARTI_SPHERE, 
	EI_PARTI_DISC, 
	EI_PARTI_QUAD, 
	EI_PARTI_COUNT, 
};

/** The approximation methods.
 */
enum {
	EI_APPROX_METHOD_REGULAR = 0, 
	EI_APPROX_METHOD_LENGTH, 
	EI_APPROX_METHOD_TYPE_COUNT, 
};

/** The subdivision schemes for subdivision surfaces.
 */
enum {
	EI_SUBDIV_SCHEME_AUTO = 0,		/**< automatically select an appropriate scheme */
	EI_SUBDIV_SCHEME_CATMARK,		/**< Catmull-Clark subdivison scheme */
	EI_SUBDIV_SCHEME_LOOP,			/**< Loop subdivision scheme */
	EI_SUBDIV_SCHEME_BILINEAR,		/**< Bilinear subdivision scheme */
	EI_SUBDIV_SCHEME_TYPE_COUNT, 
};

/** The DAG element types.
 */
enum {
	EI_ELEMENT_NONE = 0, 
	EI_ELEMENT_APPROX = (1 << 0), 
	EI_ELEMENT_INSTANCE = (1 << 1), 
	EI_ELEMENT_INSTGROUP = (1 << 2), 
	EI_ELEMENT_OPTIONS = (1 << 3), 
	EI_ELEMENT_OUTVAR = (1 << 4), 
	EI_ELEMENT_OUTPUT = (1 << 5), 
	EI_ELEMENT_CAMERA = (1 << 6), 
	EI_ELEMENT_MATERIAL = (1 << 7), 
	EI_ELEMENT_LIGHT = (1 << 8), 
	EI_ELEMENT_OBJECT = (1 << 9), 
	EI_ELEMENT_OSL_NODE = (1 << 10), 
	EI_ELEMENT_OSL_SHADERGROUP = (1 << 11), 
	EI_ELEMENT_CPP_SHADER = (1 << 12), 
};

/** The image data types.
 */
enum {
	EI_IMG_DATA_NONE = 0, 
	EI_IMG_DATA_RGB, 
	EI_IMG_DATA_RGBA, 
	EI_IMG_DATA_TYPE_COUNT, 
};

/** The pass types.
 */
#define EI_PASS_GI_CACHE_PROGRESSIVE		-5
#define EI_PASS_FINAL_FRAME					-3
#define EI_PASS_PROGRESSIVE_INITIAL0		-2
#define EI_PASS_PROGRESSIVE_INITIAL			-1
#define EI_PASS_CLEAR_FRAME					EI_MAX_INT

/** Compressed data size.
 */
#define EI_CVECTOR_SIZE		3 /* bytes */
#define EI_HVECTOR_SIZE		6 /* bytes */
#define EI_HVECTOR2_SIZE	4 /* bytes */

/* Parser 2 */
EI_API eiBool ei_parse2(const char *filename, eiBool ignore_render = EI_FALSE);
EI_API eiBool ei_parse_buffer2(const char *buffer, eiSizet buffer_size, eiBool ignore_render = EI_FALSE);
EI_API eiBool ei_parse_bbox2(const char *filename, const char *root_instgroup, eiBBox *bbox, eiBBox *motion_bbox);

EI_API eiUint32 ei_base85_calc_encode_bound(eiUint32 input_length);
EI_API eiUint32 ei_base85_encode(const eiByte *data, eiUint32 input_length, char *encoded_data);
EI_API eiUint32 ei_base85_calc_decode_bound(eiUint32 input_length);
EI_API eiUint32 ei_base85_decode(const char *data, eiUint32 input_length, eiByte *decoded_data);

/* Parser */
EI_API eiBool ei_get_ess_host_app(const char *filename, char *host_app_name);

typedef void (*eiESSFileRefCallback)(const char *ref_filename);
EI_API eiBool ei_get_ess_file_refs(const char *filename, eiESSFileRefCallback cb);

/* Exporter */
EI_API eiBool ei_echo_file(const char *filename);
EI_API eiBool ei_echo(eiBool value);
EI_API eiBool ei_binary_encoding(eiBool value);
EI_API eiBool ei_export_file(const char *filename, const char *root_instgroup, const char *camera_inst, const char *options);

/* Package */
EI_API eiBool ei_make_package(
	const char *ESS_filename, 
	const char *ZIP_filename);
EI_API eiBool ei_extract_package(
	const char *ZIP_filename, 
	const char *ESS_filename);

/* Query */
EI_API void ei_print_nodes();
EI_API void ei_print_node_info(const char *node_name);

/* Rendering context */
EI_API void ei_context();
EI_API void ei_end_context();
EI_API void ei_sub_context();
EI_API void ei_end_sub_context();
EI_API eiNode *ei_context_current_node();
EI_API void ei_handle_exception();

/* Namespace */
EI_API void ei_namespace(const char *namespace_name);
EI_API void ei_end_namespace();

/* Node description */
EI_API void ei_node_desc(const char *desc_name);

EI_API void ei_add_token(
	const char *param_name, 
	const char *default_value);
EI_API void ei_add_int(
	const char *param_name, 
	const eiInt default_value);
EI_API void ei_add_enum(
	const char *param_name, 
	const eiInt default_value, 
	const char **enum_names);
EI_API void ei_add_scalar(
	const char *param_name, 
	const eiScalar default_value);
EI_API void ei_add_color(
	const char *param_name, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_add_vector(
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_add_point(
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_add_tag(
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_add_node(
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_add_array(
	const char *param_name, 
	const eiInt param_array_type, 
	const eiTag default_value);
EI_API void ei_add_index(
	const char *param_name, 
	const eiIndex default_value);
EI_API void ei_add_bool(
	const char *param_name, 
	const eiBool default_value);

EI_API void ei_add_out_token(
	const char *param_name, 
	const char *default_value);
EI_API void ei_add_out_int(
	const char *param_name, 
	const eiInt default_value);
EI_API void ei_add_out_scalar(
	const char *param_name, 
	const eiScalar default_value);
EI_API void ei_add_out_color(
	const char *param_name, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_add_out_vector(
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_add_out_point(
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_add_out_tag(
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_add_out_node(
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_add_out_array(
	const char *param_name, 
	const eiInt param_array_type, 
	const eiTag default_value);
EI_API void ei_add_out_index(
	const char *param_name, 
	const eiIndex default_value);
EI_API void ei_add_out_bool(
	const char *param_name, 
	const eiBool default_value);

EI_API void ei_end_node_desc();

/* Node parameter declaration */
EI_API void ei_declare_token(
	const char *name, 
	const eiInt storage_class, 
	const char *default_value);
EI_API void ei_declare_byte(
	const char *name, 
	const eiInt storage_class, 
	const eiByte default_value);
EI_API void ei_declare_int(
	const char *name, 
	const eiInt storage_class, 
	const eiInt default_value);
EI_API void ei_declare_scalar(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar default_value);
EI_API void ei_declare_color(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_declare_vector(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_declare_point(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_declare_vector2(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar x, const eiScalar y);
EI_API void ei_declare_vector4(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_declare_bbox(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_declare_tag(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_node(
	const char *name, 
	const eiInt storage_class, 
	const char *node_name);
EI_API void ei_declare_index(
	const char *name, 
	const eiInt storage_class, 
	const eiIndex default_value);
EI_API void ei_declare_bool(
	const char *name, 
	const eiInt storage_class, 
	const eiBool default_value);
EI_API void ei_declare_matrix(
	const char *name, 
	const eiInt storage_class, 
	const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
	const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
	const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
	const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);

EI_API void ei_declare_token_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_byte_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_int_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_scalar_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_color_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_vector_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_point_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_vector2_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_vector4_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_cvector_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_hvector_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_hvector2_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_bbox_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_node_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_index_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_bool_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);
EI_API void ei_declare_matrix_array(
	const char *name, 
	const eiInt storage_class, 
	const eiTag default_value);

/* Node parameter editing */
EI_API void ei_param_token(
	const char *param_name, 
    const char *param_value);
EI_API void ei_param_byte(
	const char *param_name, 
    const eiByte param_value);
EI_API void ei_param_int(
	const char *param_name, 
    const eiInt param_value);
EI_API void ei_param_enum(
	const char *param_name, 
	const char *param_value);
EI_API void ei_param_scalar(
	const char *param_name, 
    const eiScalar param_value);
EI_API void ei_param_color(
	const char *param_name, 
    const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_param_vector(
	const char *param_name, 
    const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_param_point(
	const char *param_name, 
    const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_param_vector2(
	const char *param_name, 
    const eiScalar x, const eiScalar y);
EI_API void ei_param_vector4(
	const char *param_name, 
    const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_param_bbox(
	const char *param_name, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_param_tag(
	const char *param_name, 
	const eiTag param_value);
EI_API void ei_param_node(
	const char *param_name, 
	const char *node_name);
EI_API void ei_param_array(
	const char *param_name, 
	const eiTag param_value);
EI_API void ei_param_index(
	const char *param_name, 
	const eiIndex param_value);
EI_API void ei_param_bool(
	const char *param_name, 
	const eiBool param_value);
EI_API void ei_param_matrix(
	const char *param_name, 
	const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
	const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
	const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
	const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);
EI_API void ei_param_link(
    const char *param_name, 
    const char *src_shader_name, 
    const char *src_param_name);

/* Plug-in linking */
EI_API void ei_link(const char *filename);

/* Commands */
EI_API void ei_verbose(const char *level);
EI_API void ei_delete(const char *element_name);

EI_API void ei_render_prepare();
EI_API void ei_render_run(const char *root_instgroup, const char *camera_inst, const char *options);
EI_API void ei_render_cleanup();
EI_API eiTag ei_render_get_framebuffer(const char *name);

EI_API void ei_render_run_begin(const char *root_instgroup, const char *camera_inst, const char *options);
EI_API void ei_render_run_frame();
EI_API void ei_render_run_end();

EI_API void ei_tessellate_begin(const char *root_instgroup, const char *camera_inst, const char *options);
EI_API void ei_tessellate_end();

struct eiRenderParameters {
	char root_instgroup[EI_MAX_NODE_NAME_LEN];
	char camera_inst[EI_MAX_NODE_NAME_LEN];
	char options[EI_MAX_NODE_NAME_LEN];
};

EI_API eiBool ei_get_last_render_params(eiRenderParameters *render_params);

EI_API void ei_echo_render(const char *root_instgroup, const char *camera_inst, const char *options);

EI_API void ei_transform(
	const char *name, 
	const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
	const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
	const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
	const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);

/* Nodes */
EI_API void ei_declare(const char *name, const eiInt storage_class, const eiInt type, const eiInt array_type, const void *default_value);
EI_API void ei_variable(const char *name, const void *value);

EI_API void ei_node(const char *type, const char *name);
EI_API void ei_end_node();

/* Tabs */
EI_API eiTag ei_tab(const eiInt type, const eiUint nkeys);

	EI_API void ei_tab_reserve(const eiUint size);
	EI_API void ei_tab_add(const void *value);
	EI_API void ei_tab_add_token(const char *value);
	EI_API void ei_tab_add_byte(const eiByte value);
	EI_API void ei_tab_add_int(const eiInt value);
	EI_API void ei_tab_add_scalar(const eiScalar value);
	EI_API void ei_tab_add_color(const eiScalar r, const eiScalar g, const eiScalar b);
	EI_API void ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z);
	EI_API void ei_tab_add_point(const eiScalar x, const eiScalar y, const eiScalar z);
	EI_API void ei_tab_add_vector2(const eiScalar x, const eiScalar y);
	EI_API void ei_tab_add_vector4(const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
	EI_API void ei_tab_add_cvector(const eiScalar x, const eiScalar y, const eiScalar z);
	EI_API void ei_tab_add_hvector(const eiScalar x, const eiScalar y, const eiScalar z);
	EI_API void ei_tab_add_hvector2(const eiScalar x, const eiScalar y);
	EI_API void ei_tab_add_bbox(
		const eiScalar xmin, const eiScalar xmax, 
		const eiScalar ymin, const eiScalar ymax, 
		const eiScalar zmin, const eiScalar zmax);
	EI_API void ei_tab_add_tag(const eiTag value);
	EI_API void ei_tab_add_node(const char *node_name);
	EI_API void ei_tab_add_index(const eiIndex value);
	EI_API void ei_tab_add_bool(const eiBool value);
	EI_API void ei_tab_add_matrix(
		const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
		const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
		const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
		const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);

EI_API void ei_end_tab();

EI_API void ei_cvector_pack(void *dst, const eiVector *src);
EI_API void ei_cvector_unpack(eiVector *dst, const void *src);

EI_API void ei_hvector_pack(void *dst, const eiVector *src);
EI_API void ei_hvector_unpack(eiVector *dst, const void *src);

EI_API void ei_hvector2_pack(void *dst, const eiVector2 *src);
EI_API void ei_hvector2_unpack(eiVector2 *dst, const void *src);

/* Overrides */
EI_API void ei_override_token(const char *object, const char *name, const char *value);
EI_API void ei_override_bool(const char *object, const char *name, eiBool value);
EI_API void ei_override_int(const char *object, const char *name, eiInt value);
EI_API void ei_override_enum(const char *object, const char *name, const char *value);
EI_API void ei_override_scalar(const char *object, const char *name, eiScalar value);
EI_API void ei_override_color(const char *object, const char *name, eiScalar r, eiScalar g, eiScalar b);
EI_API void ei_override_vector(const char *object, const char *name, eiScalar x, eiScalar y, eiScalar z);
EI_API void ei_override_array(const char *object, const char *name, eiTag value);

/* System */
EI_API void ei_add_shader_searchpath(const char *path);
EI_API void ei_add_texture_searchpath(const char *path);
EI_API void ei_add_scene_searchpath(const char *path);
EI_API void ei_set_texture_memlimit(eiInt limit_in_megabytes);
EI_API void ei_set_texture_openfiles(eiInt max_open_files);
EI_API void ei_set_page_file_dir(const char *dir);

EI_API void ei_resolve_shader_name(char *filename, const char *shader_name);
EI_API void ei_resolve_texture_name(char *filename, const char *texture_name);
EI_API void ei_resolve_scene_name(char *filename, const char *scene_name);

EI_API void ei_clear_temp_files();
EI_API void ei_texcache_invalidate_all();

/* Display */
typedef void (*DisplayCallback)(eiInt frameWidth, eiInt frameHeight, void *param);

EI_API void ei_display(DisplayCallback display_cb, void *param, eiInt imageWidth, eiInt imageHeight);
EI_API void ei_display_clear_viewport(eiInt frameWidth, eiInt frameHeight);
EI_API void ei_display_draw_pixels(eiInt offsetX, eiInt offsetY, eiInt imageWidth, eiInt imageHeight, const eiColor *pixels);

enum {
	EI_DRAG_MODE_NONE = 0, 
	EI_DRAG_MODE_NORMAL, 
	EI_DRAG_MODE_ALT, 
	EI_DRAG_MODE_SHIFT, 
	EI_DRAG_MODE_CTRL, 
};

enum {
	EI_DRAG_BUTTON_NONE = 0, 
	EI_DRAG_BUTTON_LEFT, 
	EI_DRAG_BUTTON_RIGHT, 
	EI_DRAG_BUTTON_MIDDLE, 
};

EI_API eiInt ei_display_get_mouse_move(eiScalar * const offset, eiInt * const button);
EI_API void ei_display_get_scroll_offset(eiScalar * const offset);

/* Tone mapping */
EI_API void ei_temperature_to_color(eiScalar color_temperature, eiColor & result);

struct eiToneOp;

EI_API eiToneOp *ei_create_toneop();
EI_API void ei_delete_toneop(eiToneOp *toneop);
EI_API void ei_toneop_update(
	eiToneOp *toneop, 
	eiBool enabled, 
	eiScalar exposure_value, 
	eiScalar highlights, 
	eiScalar midtones, 
	eiScalar shadows, 
	eiScalar saturation, 
	eiScalar whitepoint);
EI_API void ei_toneop_apply(const eiToneOp *toneop, eiColor & color_result);

EI_API void ei_toneop_auto_exposure_rgb(
	eiToneOp *toneop, 
	eiScalar target_intensity, 
	const eiColor *src_img, 
	eiInt image_width, 
	eiInt image_height);
EI_API void ei_toneop_auto_exposure_rgba(
	eiToneOp *toneop, 
	eiScalar target_intensity, 
	const eiVector4 *src_img, 
	eiInt image_width, 
	eiInt image_height);

/* Post effects */
struct eiBloomOp;

EI_API eiBloomOp *ei_create_bloomop();
EI_API void ei_delete_bloomop(eiBloomOp *op);
EI_API void ei_bloomop_update(
	eiBloomOp *op, 
	eiScalar bloom_intensity,	/**< [0, inf) */
	eiScalar glare_intensity,	/**< [0, inf) */
	eiScalar threshold,			/**< [0, inf) */
	eiInt ray_count,			/**< [0, 8] */
	eiScalar glare_rotation,	/**< [0, 360] in degrees */
	eiScalar glare_blur,		/**< [0, 1] */
	eiScalar color_intensity,	/**< [0, 1] */
	eiScalar color_shift);		/**< [0, 1] */
EI_API eiBool ei_bloomop_apply(
	eiBloomOp *op, 
	eiColor *dst_img, 
	const eiColor *src_img, 
	eiInt image_width, 
	eiInt image_height);
EI_API eiBool ei_bloomop_apply_rgba(
	eiBloomOp *op, 
	eiVector4 *dst_img, 
	const eiVector4 *src_img, 
	eiInt image_width, 
	eiInt image_height);

#endif
