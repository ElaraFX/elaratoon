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

#ifndef EI_SHADERX_H
#define EI_SHADERX_H

/** Header of C++ shading APIs.
 * for consistency, please do NOT change this file.
 * \file ei_shaderx.h
 * \author Elvic Liang
 */

#include <ei.h>
#include <ei_random.h>
#include <ei_base_bucket.h>

#define eval_token(name) eval_token_func(e_##name)
#define eval_int(name) eval_int_func(e_##name)
#define eval_enum(name) eval_int_func(e_##name)
#define eval_scalar(name) eval_scalar_func(e_##name)
#define eval_color(name) eval_color_func(e_##name)
#define eval_vector(name) eval_vector_func(e_##name)
#define eval_point(name) eval_point_func(e_##name)
#define eval_normal(name) eval_normal_func(e_##name)
#define eval_tag(name) eval_tag_func(e_##name)
#define eval_node(name) eval_node_func(e_##name)
#define eval_array(name) eval_array_func(e_##name)
#define eval_index(name) eval_index_func(e_##name)
#define eval_bool(name) eval_bool_func(e_##name)

#define declare_token(name, default_value) ei_add_token(#name, default_value)
#define declare_int(name, default_value) ei_add_int(#name, default_value)
#define declare_enum(name, default_value, enum_names) ei_add_enum(#name, default_value, enum_names)
#define declare_scalar(name, default_value) ei_add_scalar(#name, default_value)
#define declare_color(name, x, y, z) ei_add_color(#name, x, y, z)
#define declare_vector(name, x, y, z) ei_add_vector(#name, x, y, z)
#define declare_point(name, x, y, z) ei_add_vector(#name, x, y, z)
#define declare_normal(name, x, y, z) ei_add_vector(#name, x, y, z)
#define declare_tag(name, default_value) ei_add_tag(#name, default_value)
#define declare_node(name, default_value) ei_add_node(#name, default_value)
#define declare_array(name, array_type, default_value) ei_add_array(#name, array_type, default_value)
#define declare_index(name, default_value) ei_add_index(#name, default_value)
#define declare_bool(name, default_value) ei_add_bool(#name, default_value)

#define declare_out_token(name, default_value) ei_add_out_token(#name, default_value)
#define declare_out_int(name, default_value) ei_add_out_int(#name, default_value)
#define declare_out_scalar(name, default_value) ei_add_out_scalar(#name, default_value)
#define declare_out_color(name, x, y, z) ei_add_out_color(#name, x, y, z)
#define declare_out_vector(name, x, y, z) ei_add_out_vector(#name, x, y, z)
#define declare_out_point(name, x, y, z) ei_add_out_vector(#name, x, y, z)
#define declare_out_normal(name, x, y, z) ei_add_out_vector(#name, x, y, z)
#define declare_out_tag(name, default_value) ei_add_out_tag(#name, default_value)
#define declare_out_node(name, default_value) ei_add_out_node(#name, default_value)
#define declare_out_array(name, array_type, default_value) ei_add_out_array(#name, array_type, default_value)
#define declare_out_index(name, default_value) ei_add_out_index(#name, default_value)
#define declare_out_bool(name, default_value) ei_add_out_bool(#name, default_value)

#define geometry(name) \
	class name : public geometry_shader {\
	public:

#define light(name) \
	class name : public light_shader {\
	public:

#define lens(name) \
	class name : public camera_shader {\
	public:

#define texture(name) \
	class name : public texture_shader {\
	public:

#define end_shader(name) \
	};\
	\
	static eiBool name##_main(\
		eiState * const state, \
		void *arg)\
	{\
		((name *)state)->main(arg);\
		return EI_TRUE;\
	}\
	static eiBool name##_init_node(\
		eiState * const state, \
		void *arg)\
	{\
		((name *)state)->init_node();\
		return EI_TRUE;\
	}\
	static eiBool name##_exit_node(\
		eiState * const state, \
		void *arg)\
	{\
		((name *)state)->exit_node();\
		return EI_TRUE;\
	}\
	\
	static void name##_deletethis(eiPluginObject *object)\
	{\
		if (object == NULL)\
		{\
			EI_ASSERT(0);\
			return;\
		}\
		\
		name::exit();\
		\
		ei_shader_exit((eiShader *)object);\
		ei_free(object);\
	}\
	\
	EI_SHADER_API void declare_##name(void *param)\
	{\
		ei_node_desc(#name);\
		name::parameters();\
		ei_end_node_desc();\
	}\
	\
	EI_SHADER_API eiPluginObject *create_##name(void *param)\
	{\
		eiShader *shader = (eiShader *)ei_allocate(sizeof(eiShader));\
		ei_shader_init(shader, EI_FALSE);\
		\
		name::init();\
		\
		((eiPluginObject *)shader)->deletethis = name##_deletethis;\
		shader->base.deferred_init_node = ei_shader_init_node;\
		shader->base.deferred_exit_node = ei_shader_exit_node;\
		shader->main = name##_main;\
		shader->init_node = name##_init_node;\
		shader->exit_node = name##_exit_node;\
		shader->size = sizeof(name);\
		\
		return ((eiPluginObject *)shader);\
	}

#define end_lens(name) \
	};\
	\
	static eiBool name##_support(\
		eiState * const state, \
		eiNode *cam, \
		eiInt feature, \
		void *feature_params)\
	{\
		return ((name *)state)->support(\
			cam, \
			feature, \
			feature_params);\
	}\
	\
	static eiBool name##_object_to_screen(\
		eiState * const state, \
		eiNode *cam, \
		eiVector *rpos, \
		const eiVector *opos, \
		const eiMatrix *object_to_view)\
	{\
		return ((name *)state)->object_to_screen(\
			cam, \
			rpos, \
			opos, \
			object_to_view);\
	}\
	\
	static eiBool name##_update_world_bbox(\
		eiState * const state, \
		eiNode *cam, \
		const eiBBox *world_bbox)\
	{\
		((name *)state)->update_world_bbox(\
			cam, \
			world_bbox);\
		return EI_TRUE;\
	}\
	\
	static eiBool name##_generate_ray(\
		eiState * const state, \
		eiNode *cam, \
		eiCameraOutput *out)\
	{\
		return ((name *)state)->generate_ray(\
			cam, \
			out);\
	}\
	static eiBool name##_init_node(\
		eiState * const state, \
		void *arg)\
	{\
		((name *)state)->init_node();\
		return EI_TRUE;\
	}\
	static eiBool name##_exit_node(\
		eiState * const state, \
		void *arg)\
	{\
		((name *)state)->exit_node();\
		return EI_TRUE;\
	}\
	\
	static void name##_deletethis(eiPluginObject *object)\
	{\
		if (object == NULL)\
		{\
			EI_ASSERT(0);\
			return;\
		}\
		\
		name::exit();\
		\
		ei_camera_shader_exit((eiCameraShader *)object);\
		ei_free(object);\
	}\
	\
	EI_SHADER_API void declare_##name(void *param)\
	{\
		ei_node_desc(#name);\
		name::parameters();\
		ei_end_node_desc();\
	}\
	\
	EI_SHADER_API eiPluginObject *create_##name(void *param)\
	{\
		eiCameraShader *shader = (eiCameraShader *)ei_allocate(sizeof(eiCameraShader));\
		ei_camera_shader_init(shader, EI_FALSE);\
		\
		name::init();\
		\
		((eiPluginObject *)shader)->deletethis = name##_deletethis;\
		shader->base.base.deferred_init_node = ei_camera_shader_init_node;\
		shader->base.base.deferred_exit_node = ei_camera_shader_exit_node;\
		shader->support = name##_support;\
		shader->object_to_screen = name##_object_to_screen;\
		shader->update_world_bbox = name##_update_world_bbox;\
		shader->generate_ray = name##_generate_ray;\
		shader->base.init_node = name##_init_node;\
		shader->base.exit_node = name##_exit_node;\
		shader->base.size = sizeof(name);\
		\
		return ((eiPluginObject *)shader);\
	}

class base_shader : public eiState {
protected:
	inline eiNode *options()
	{
		return ei_state_options(this);
	}

	inline eiNode *camera()
	{
		return ei_state_camera(this);
	}

	inline eiBool call_shader(
		const eiTag shader, 
		const eiUint num_outputs, 
		eiShaderOutput *outputs, 
		void *arg)
	{
		return ei_call_shader(
			this, 
			shader, 
			num_outputs, 
			outputs, 
			arg);
	}

	inline eiScalar random()
	{
		return ei_state_random(this);
	}

	inline eiVector2 raster_pos()
	{
		eiVector2 r;

		ei_raster_pos(&r, this);

		return r;
	}

	inline eiPoint to_camera(const eiPoint & v)
	{
		eiPoint r;

		ei_to_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vto_camera(const eiVector & v)
	{
		eiVector r;

		ei_vto_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nto_camera(const eiNormal & v)
	{
		eiNormal r;

		ei_nto_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiPoint from_camera(const eiPoint & v)
	{
		eiPoint r;

		ei_from_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vfrom_camera(const eiVector & v)
	{
		eiVector r;

		ei_vfrom_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nfrom_camera(const eiNormal & v)
	{
		eiNormal r;

		ei_nfrom_camera((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiPoint to_object(const eiPoint & v)
	{
		eiPoint r;

		ei_to_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vto_object(const eiVector & v)
	{
		eiVector r;

		ei_vto_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nto_object(const eiNormal & v)
	{
		eiNormal r;

		ei_nto_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiPoint from_object(const eiPoint & v)
	{
		eiPoint r;

		ei_from_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vfrom_object(const eiVector & v)
	{
		eiVector r;

		ei_vfrom_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nfrom_object(const eiNormal & v)
	{
		eiNormal r;

		ei_nfrom_object((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiPoint to_world(const eiPoint & v)
	{
		eiPoint r;

		ei_to_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vto_world(const eiVector & v)
	{
		eiVector r;

		ei_vto_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nto_world(const eiNormal & v)
	{
		eiNormal r;

		ei_nto_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiPoint from_world(const eiPoint & v)
	{
		eiPoint r;

		ei_from_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiVector vfrom_world(const eiVector & v)
	{
		eiVector r;

		ei_vfrom_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiNormal nfrom_world(const eiNormal & v)
	{
		eiNormal r;

		ei_nfrom_world((eiVector *)(&r), (const eiVector *)(&v), this);

		return r;
	}

	inline eiBool msg_get(const char *name, eiToken & value)
	{
		return ei_msg_get_token(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiInt & value)
	{
		return ei_msg_get_int(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiScalar & value)
	{
		return ei_msg_get_scalar(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiColor & value)
	{
		return ei_msg_get_color(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiVector & value)
	{
		return ei_msg_get_vector(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiVector4 & value)
	{
		return ei_msg_get_vector4(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiTag & value)
	{
		return ei_msg_get_tag(this, name, &value);
	}

	inline eiBool msg_get(const char *name, eiIndex & value)
	{
		return ei_msg_get_index(this, name, &value);
	}

	//inline eiBool msg_get(const char *name, eiBool & value)
	//{
		//return ei_msg_get_bool(this, name, &value);
	//}

	inline eiBool msg_set(const char *name, eiToken value)
	{
		return ei_msg_set_token(this, name, value);
	}

	inline eiBool msg_set(const char *name, const eiInt value)
	{
		return ei_msg_set_int(this, name, value);
	}

	inline eiBool msg_set(const char *name, const eiScalar value)
	{
		return ei_msg_set_scalar(this, name, value);
	}

	inline eiBool msg_set(const char *name, const eiColor & value)
	{
		return ei_msg_set_color(this, name, &value);
	}

	inline eiBool msg_set(const char *name, const eiVector & value)
	{
		return ei_msg_set_vector(this, name, &value);
	}

	inline eiBool msg_set(const char *name, const eiVector4 & value)
	{
		return ei_msg_set_vector4(this, name, &value);
	}

	inline eiBool msg_set(const char *name, const eiTag value)
	{
		return ei_msg_set_tag(this, name, value);
	}

	inline eiBool msg_set(const char *name, const eiIndex value)
	{
		return ei_msg_set_index(this, name, value);
	}

	//inline eiBool msg_set(const char *name, const eiBool value)
	//{
		//return ei_msg_set_bool(this, name, value);
	//}
};

class eval_ops : public base_shader {
protected:
	inline eiToken & eval_token_func(const eiIndex e_name)
	{
		return *ei_eval_token(this, e_name);
	}

	inline eiInt & eval_int_func(const eiIndex e_name)
	{
		return *ei_eval_int(this, e_name);
	}

	inline eiScalar & eval_scalar_func(const eiIndex e_name)
	{
		return *ei_eval_scalar(this, e_name);
	}

	inline eiColor & eval_color_func(const eiIndex e_name)
	{
		return *ei_eval_color(this, e_name);
	}

	inline eiVector & eval_vector_func(const eiIndex e_name)
	{
		return *ei_eval_vector(this, e_name);
	}

	inline eiPoint & eval_point_func(const eiIndex e_name)
	{
		return *ei_eval_point(this, e_name);
	}

	inline eiNormal & eval_normal_func(const eiIndex e_name)
	{
		return *ei_eval_normal(this, e_name);
	}

	inline eiTag & eval_tag_func(const eiIndex e_name)
	{
		return *ei_eval_tag(this, e_name);
	}

	inline eiTag & eval_node_func(const eiIndex e_name)
	{
		return *ei_eval_node(this, e_name);
	}

	inline eiTag & eval_array_func(const eiIndex e_name)
	{
		return *ei_eval_array(this, e_name);
	}

	inline eiIndex & eval_index_func(const eiIndex e_name)
	{
		return *ei_eval_index(this, e_name);
	}

	inline eiBool & eval_bool_func(const eiIndex e_name)
	{
		return *ei_eval_bool(this, e_name);
	}
};

class geometry_shader : public eval_ops {
protected:
	inline void geometry_root(const char *name)
	{
		ei_geometry_root(name, this);
	}
};

class light_shader : public eval_ops {
};

class camera_shader : public eval_ops {
};

class texture_shader : public eval_ops {
};

class light_filter_interface {
public:
	eiScalar m_max_intensity;
	inline light_filter_interface() : m_max_intensity(1.0f) {}
};

#endif
