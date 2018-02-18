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

#ifndef EI_NODESYS_H
#define EI_NODESYS_H

/** Pluggable node system for scene management.
 * \file ei_nodesys.h
 * \author Elvic Liang
 */

#include <ei_api.h>
#include <ei_dataflow.h>
#include <ei_plugsys.h>
#include <ei_symbol.h>
#include <ei_color.h>
#include <ei_vector.h>
#include <ei_vector2.h>
#include <ei_vector4.h>
#include <ei_matrix.h>

#include <string>

/** Storage class of primitive variables.
 */
enum {
	EI_CONSTANT = 0,	/**< constant per-object */
	EI_UNIFORM,			/**< constant per-primitive */
	EI_VARYING,			/**< bilinearly interpolated */
	EI_FACEVARYING,		/**< bilinearly interpolated within each face */
	EI_VERTEX, 			/**< for internal use only */
};

typedef struct eiNodeObject		eiNodeObject;
typedef struct eiNodeDesc		eiNodeDesc;
typedef struct eiNode			eiNode;
typedef struct eiNodeUserShader	eiNodeUserShader;

/** The custom node constructor for derived classes.
 */
typedef void (*ei_node_init_func)(eiNode *node);
/** The custom deferred node constructor for derived classes. it's called 
 * at the first time when this node is accessed.
 */
typedef void (*ei_node_deferred_init_func)(eiNode *node);
/** The custom node destructor for derived classes.
 */
typedef void (*ei_node_exit_func)(eiNode *node);
/** The custom deferred node destructor for derived classes.
 */
typedef void (*ei_node_deferred_exit_func)(eiNode *node);

/** Node object loaded from DSO. it's 
 * derived from plugin object.
 */
struct eiNodeObject {
	eiPluginObject				base;				/**< the base plugin object */
	ei_node_init_func			init_node;			/**< the node constructor */
	ei_node_deferred_init_func	deferred_init_node;	/**< the deferred node constructor */
	ei_node_exit_func			exit_node;			/**< the node destructor */
	ei_node_deferred_exit_func	deferred_exit_node;	/**< the deferred node destructor */
	eiInt						type;				/**< the user type flag, will not be modified by the node 
														 system */
	eiBool						is_volume;			/**< whether this node is a volume object */
};

/** Parameter value.
 */
struct eiParamValue {
	union {
		eiToken			as_token;			/**< token value */
		eiByte			as_byte;			/**< byte value */
		eiInt			as_int;				/**< integer value */
		eiScalar		as_scalar;			/**< scalar value */
		eiColor			as_color;			/**< color value */
		eiVector		as_vector;			/**< vector value */
		eiPoint			as_point;			/**< point value */
		eiVector2		as_vector2;			/**< vector2 value */
		eiVector4		as_vector4;			/**< vector4 value */
		eiTag			as_tag;				/**< tag value */
		eiTag			as_node;			/**< node value */
		eiTag			as_array;			/**< array value */
		eiIndex			as_index;			/**< index value */
		eiBool			as_bool;			/**< bool value */
		eiUint			as_offset;			/**< the offset of this parameter in node parameter 
												 table in bytes NOT including encoded parameter 
												 values */
	};
};

/** Node parameter.
 */
struct eiNodeParam {
	const char			*unique_name;		/**< the unique name on local host */
	eiByte				storage_class;		/**< the storage class of the node parameter */
	eiByte				is_builtin;			/**< the parameter is a built-in parameter */
	eiShort				type;				/**< the data type of the node parameter */
	eiUint				size;				/**< the size of this parameter in bytes */
	eiUint				full_offset;		/**< the offset in parameter table in bytes including 
											     encoded parameter values */
	eiTag				inst;				/**< the connected node instance tag as the input */
	eiIndex				param;				/**< the connected node parameter index as the input */
	eiShort				channel_offset;		/**< the offset in vertex channels for varying 
											     or face-varying primitive variables */
	eiShort				array_type;			/**< data type of array elements */
	eiTag				constant_names;		/**< constant names if this is enum or bitfields */
	eiIndex				fvar_idx_param;		/**< parameter index of _idx when it's face-varying */
	eiShort				is_internal;		/**< this parameter is an internal parameter */
	eiShort				padding;
	eiParamValue		value;				/**< the parameter value */
};

/** Node instance in scene DAG.
 */
struct eiNode {
	const char			*unique_name;			/**< the unique resolved name on local host */
	eiTag				tag;					/**< the tag of this node in database */
	eiTag				desc;					/**< the tag of the node desc */
	eiInt				num_params;				/**< the number of all node parameters */
	eiUint				full_param_table_size;	/**< the size of all node parameters for this node instance 
												     in bytes, NOT including connected node parameters, 
												     including encoded parameter values */
	eiUint				param_table_size;		/**< the size of all node parameters for this node instance 
												     in bytes, not including connected node parameters, 
													 NOT including encoded parameter values */
	eiTokenMap			param_table;			/**< the local symbol map for node parameters */
	eiNodeObject		*object;				/**< this node plugin object must be re-created for 
												     each local host. this is copied from eiNodeDesc to each 
												     node instance to support per-node data */
	void				*glob;					/**< the pointer for holding current globals */
	eiIndex				uv_index;				/**< cached parameter index for uv */
	eiInt				uv_type;				/**< can be EI_TYPE_VECTOR2, EI_TYPE_VECTOR, EI_TYPE_HVECTOR2, EI_TYPE_HVECTOR */
	eiInt				uv_storage_class;		/**< can be either EI_VARYING or EI_FACEVARYING */
	eiIndex				dPdu_index, dPdv_index;	/**< cached parameter index for dPdu, dPdv */
	eiInt				dPdu_type;				/**< can be EI_TYPE_VECTOR, EI_TYPE_CVECTOR */
	eiInt				dPdu_storage_class;		/**< can be either EI_VARYING or EI_FACEVARYING */
	eiIndex				mtl_index_index;		/**< cached parameter index for mtl_index */
	eiUint				time;					/**< last edited time */
	eiTag				user_tag;				/**< tag for user data (mainly for shading system) */
	eiNodeUserShader	*user_shader;			/**< for Open Shading Language (OSL) only */
	eiInt				user_shader_flags;		/**< flags for Open Shading Language (OSL) only */
	ei_array			input_nodes;			/**< dependent nodes connected as inputs to this node */
};

/** Get the number of registered node descriptions.
 */
EI_API eiUint ei_nodesys_num_node_descs();

/** Get the i-th registered node description.
 */
EI_API const char *ei_nodesys_get_node_desc(eiUint i);

/** Get the number of first-level nodes, including deleted nodes.
 */
EI_API eiUint ei_nodesys_num_nodes();
/** Get the i-th node.
 * the returned node tag can be EI_NULL_TAG if it's already deleted.
 */
EI_API eiTag ei_nodesys_get_node(eiUint i);

/** Get the plugin system for the node system.
 */
EI_API eiPluginSystem *ei_nodesys_plugin_system();

/** Get the name of a node description.
 */
EI_API const char *ei_node_desc_name(eiNodeDesc *desc);
/** Get node description type.
 */
EI_API eiInt ei_node_desc_type(eiNodeDesc *desc);
/** Get node description type name.
 */
EI_API const char *ei_node_desc_type_name(eiNodeDesc *desc);
/** Get node object from node description.
 */
EI_API eiNodeObject *ei_node_desc_object(eiNodeDesc *desc);
/** Find a node desc by name.
 */
EI_API eiTag ei_find_node_desc(const char *desc_name);
/** Begin editing a node desc, create it if 
 * it's not created before.
 */
EI_API eiNodeDesc *ei_edit_node_desc(const char *desc_name);
/** Get the number of parameters.
 */
EI_API eiUint ei_node_desc_param_count(eiNodeDesc *desc);
/** Find a parameter by name.
 */
EI_API eiIndex ei_node_desc_find_param(
	eiNodeDesc *desc, 
	const char *param_name);
/** Get parameter name by index.
 */
EI_API const char *ei_node_desc_param_name(eiNodeDesc *desc, eiIndex param_index);
/** Get parameter type by index.
 */
EI_API eiInt ei_node_desc_param_type(eiNodeDesc *desc, eiIndex param_index);
/** Get array parameter type by index.
 */
EI_API eiInt ei_node_desc_param_array_type(eiNodeDesc *desc, eiIndex param_index);
/** Get parameter default. User must call ei_db_end on the returned tag 
 * after using the default value.
 */
EI_API void *ei_node_desc_param_default(eiNodeDesc *desc, eiIndex param_index, eiTag * const tag);
/** Get enum strings.
 */
EI_API eiTag ei_node_desc_param_enums(eiNodeDesc *desc, eiIndex param_index);
/** Add a parameter to a node desc.
 */
EI_API void ei_node_desc_add_param(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const eiInt param_type, 
	const eiInt param_array_type, 
	const char *param_name, 
	const void *default_value);
/** Add bitfields parameter to a node desc.
 */
EI_API void ei_node_desc_add_bitfields(
	eiNodeDesc *desc, 
	const char *param_name, 
	const eiInt default_value, 
	const char **bitfield_names);
/** Add an enum parameter to a node desc.
 */
EI_API void ei_node_desc_add_enum(
	eiNodeDesc *desc, 
	const char *param_name, 
	const eiInt default_value, 
	const char **enum_names);
/** Add an output parameter to a node desc.
 */
EI_API void ei_node_desc_add_out_param(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const eiInt param_type, 
	const eiInt param_array_type, 
	const char *param_name, 
	const void *default_value);
/** End editing a node desc.
 */
EI_API void ei_end_edit_node_desc(eiNodeDesc *desc);
/** Delete a node desc.
 */
EI_API void ei_delete_node_desc(const eiTag tag);

/** Find a node by name.
 */
EI_API eiTag ei_find_node(const char *node_name);
/** Find a node parameter by name in parameter list, 
 * this function can be used while editing the node, but it's 
 * slower.
 */
EI_API eiIndex ei_node_find_param(
	eiNode *node, 
	const char *param_name);
/** Lookup a node parameter by name from symbol table. 
 * this function should NOT be called when editing the node, 
 * because symbol table has not been re-built while editing.
 * \param unique_param_name It must be a unique string.
 */
EI_API eiIndex ei_node_lookup_param(
	eiNode *node, 
	const char *unique_param_name);
/** Clone a node.
 */
EI_API eiTag ei_clone_node(const eiTag src_tag);
/** Begin editing a node, create it if it's not 
 * created before.
 */
EI_API eiNode *ei_edit_node(
	const char *node_name, 
	eiBool *need_init);
/** When a node is shared, the node and all its input nodes 
 * will be evaluated once. otherwise, the node and all its 
 * inpout nodes will be evaluated multiple times for each 
 * outgoing connection.
 */
EI_API void ei_node_set_shared(eiNode *node, eiBool value);
/** Get if the node is shared.
 */
EI_API eiBool ei_node_get_shared(const eiNode *node);
/** Declare a user parameter specific for the node, returns 
 * the index of the new node parameter.
 */
EI_API eiIndex ei_node_declare_param(
	eiNode **node, 
	const eiInt param_storage_class, 
	const eiInt param_type, 
	const eiInt param_array_type, 
	const char *param_name, 
	const void *default_value);
/** Set a parameter value by index.
 */
EI_API void ei_node_set_value(
	eiNode *node, 
	const char *param_name, 
	const eiIndex param_index);
/** Set a parameter value by name.
 */
EI_API void ei_node_value(
	eiNode *node, 
	const char *param_name, 
	const void *param_value);
/** Link an input parameter to the output parameter of 
 * another node. the value of the linked parameter will depend 
 * on the source parameter.
 */
EI_API void ei_node_link(
	eiNode *node, 
	const char *param_name, 
    const char *src_node_name, 
    const char *src_param_name);
/** Unlink a parameter.
 */
EI_API void ei_node_unlink(
	eiNode *node, 
	const char *param_name);
/** Get the number of parameters in a node.
 */
EI_API eiUint ei_node_param_count(eiNode *node);
/** Begin reading a parameter information by index.
 */
EI_API eiNodeParam *ei_node_read_param(
	eiNode *node, 
	const eiIndex param_index);
/** Begin writing a parameter information by index.
 */
EI_API eiNodeParam *ei_node_write_param(
	eiNode *node, 
	const eiIndex param_index);
/** Get the constant parameter value by index.
 */
EI_API void *ei_node_get_value(
	eiNode *node, 
	const eiIndex param_index);
/** End editing a node.
 */
EI_API void ei_end_edit_node(eiNode *node);
/** Delete a node.
 */
EI_API eiBool ei_delete_node(const char *node_name);

/* Parameter declarations */
EI_API void ei_node_desc_add_token(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const char *default_value);
EI_API void ei_node_desc_add_byte(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiByte default_value);
EI_API void ei_node_desc_add_int(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiInt default_value);
EI_API void ei_node_desc_add_scalar(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar default_value);
EI_API void ei_node_desc_add_color(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_node_desc_add_vector(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_desc_add_point(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_desc_add_vector2(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y);
EI_API void ei_node_desc_add_vector4(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_node_desc_add_bbox(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_node_desc_add_matrix(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
	const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
	const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
	const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);
EI_API void ei_node_desc_add_tag(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_node(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_array(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const eiInt param_array_type, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_index(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiIndex default_value);
EI_API void ei_node_desc_add_bool(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiBool default_value);

EI_API void ei_node_desc_add_out_token(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const char *default_value);
EI_API void ei_node_desc_add_out_byte(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiByte default_value);
EI_API void ei_node_desc_add_out_int(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiInt default_value);
EI_API void ei_node_desc_add_out_scalar(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar default_value);
EI_API void ei_node_desc_add_out_color(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_node_desc_add_out_vector(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_desc_add_out_point(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_desc_add_out_vector2(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y);
EI_API void ei_node_desc_add_out_vector4(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_node_desc_add_out_bbox(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_node_desc_add_out_matrix(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiScalar m00, const eiScalar m01, const eiScalar m02, const eiScalar m03, 
	const eiScalar m10, const eiScalar m11, const eiScalar m12, const eiScalar m13, 
	const eiScalar m20, const eiScalar m21, const eiScalar m22, const eiScalar m23, 
	const eiScalar m30, const eiScalar m31, const eiScalar m32, const eiScalar m33);
EI_API void ei_node_desc_add_out_tag(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_out_node(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_out_array(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const eiInt param_array_type, 
	const char *param_name, 
	const eiTag default_value);
EI_API void ei_node_desc_add_out_index(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiIndex default_value);
EI_API void ei_node_desc_add_out_bool(
	eiNodeDesc *desc, 
	const eiInt param_storage_class, 
	const char *param_name, 
	const eiBool default_value);

EI_API eiBool ei_node_param_is_linked(const eiNodeParam* node_param);

/* Parameter readers
 * these functions should NOT be called when editing the node, 
 * because symbol table has not been re-built while editing.
 */
EI_API eiBool ei_node_get_linked(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiToken ei_node_get_token(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiByte ei_node_get_byte(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiInt ei_node_get_int(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiTag ei_node_get_enum(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiUint ei_node_get_num_bits(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiTag ei_node_get_bit_name(
	eiNode *node, 
	const eiIndex param_index, 
	const eiIndex bit_index);
EI_API eiBool ei_node_get_bit_value(
	eiNode *node, 
	const eiIndex param_index, 
	const eiIndex bit_index);
EI_API eiScalar ei_node_get_scalar(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiColor *ei_node_get_color(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiVector *ei_node_get_vector(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiPoint *ei_node_get_point(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiVector2 *ei_node_get_vector2(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiVector4 *ei_node_get_vector4(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiBBox *ei_node_get_bbox(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiTag ei_node_get_tag(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiTag ei_node_get_node(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiTag ei_node_get_array(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiIndex ei_node_get_index(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiBool ei_node_get_bool(
	eiNode *node, 
	const eiIndex param_index);
EI_API eiMatrix *ei_node_get_matrix(
	eiNode *node, 
	const eiIndex param_index);

/* Parameter writers */
EI_API void ei_node_set_token(
	eiNode *node, 
	const eiIndex param_index, 
	const char *param_value);
EI_API void ei_node_set_byte(
	eiNode *node, 
	const eiIndex param_index, 
	const eiByte param_value);
EI_API void ei_node_set_int(
	eiNode *node, 
	const eiIndex param_index, 
	const eiInt param_value);
EI_API void ei_node_set_enum(
	eiNode *node, 
	const eiIndex param_index, 
	const char *param_value);
EI_API void ei_node_set_scalar(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar param_value);
EI_API void ei_node_set_color(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_node_set_vector(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_set_point(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_set_vector2(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar x, const eiScalar y);
EI_API void ei_node_set_vector4(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_node_set_bbox(
	eiNode *node, 
	const eiIndex param_index, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_node_set_tag(
	eiNode *node, 
	const eiIndex param_index, 
	const eiTag param_value);
EI_API void ei_node_set_node(
	eiNode *node, 
	const eiIndex param_index, 
	const eiTag param_value);
EI_API void ei_node_set_array(
	eiNode *node, 
	const eiIndex param_index, 
	const eiTag param_value);
EI_API void ei_node_set_index(
	eiNode *node, 
	const eiIndex param_index, 
	const eiIndex param_value);
EI_API void ei_node_set_bool(
	eiNode *node, 
	const eiIndex param_index, 
	const eiBool param_value);
EI_API void ei_node_set_matrix(
	eiNode *node, 
	const eiIndex param_index, 
	const eiMatrix *param_value);

/* Parameter writers by name */
EI_API void ei_node_token(
	eiNode *node, 
	const char *param_name, 
	const char *param_value);
EI_API void ei_node_byte(
	eiNode *node, 
	const char *param_name, 
	const eiByte param_value);
EI_API void ei_node_int(
	eiNode *node, 
	const char *param_name, 
	const eiInt param_value);
EI_API void ei_node_enum(
	eiNode *node, 
	const char *param_name, 
	const char *param_value);
EI_API void ei_node_scalar(
	eiNode *node, 
	const char *param_name, 
	const eiScalar param_value);
EI_API void ei_node_color(
	eiNode *node, 
	const char *param_name, 
	const eiScalar r, const eiScalar g, const eiScalar b);
EI_API void ei_node_vector(
	eiNode *node, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_point(
	eiNode *node, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z);
EI_API void ei_node_vector2(
	eiNode *node, 
	const char *param_name, 
	const eiScalar x, const eiScalar y);
EI_API void ei_node_vector4(
	eiNode *node, 
	const char *param_name, 
	const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
EI_API void ei_node_bbox(
	eiNode *node, 
	const char *param_name, 
	const eiScalar xmin, const eiScalar xmax, 
	const eiScalar ymin, const eiScalar ymax, 
	const eiScalar zmin, const eiScalar zmax);
EI_API void ei_node_tag(
	eiNode *node, 
	const char *param_name, 
	const eiTag param_value);
EI_API void ei_node_node(
	eiNode *node, 
	const char *param_name, 
	const eiTag param_value);
EI_API void ei_node_array(
	eiNode *node, 
	const char *param_name, 
	const eiTag param_value);
EI_API void ei_node_index(
	eiNode *node, 
	const char *param_name, 
	const eiIndex param_value);
EI_API void ei_node_bool(
	eiNode *node, 
	const char *param_name, 
	const eiBool param_value);
EI_API void ei_node_matrix(
	eiNode *node, 
	const char *param_name, 
	const eiMatrix *param_value);

EI_API const char *ei_get_node_name_by_tag(const eiTag tag);

#endif
