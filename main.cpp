
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include Elara SDK
#include <ei.h>
#include <ei_raytracer.h>
#include <ei_data_table.h>

// Really hacky here, exposing Elara's internal data structures...
// However, it does not matter that much because we can always 
// keep this binary compatible...
struct eiRaySubscene
{
	eiTag			object_instances;
};

static void rprocess_pass_started(eiProcess *process, eiInt pass_id)
{
}

static void rprocess_pass_finished(eiProcess *process, eiInt pass_id)
{
}

static void rprocess_job_started(
	eiProcess *process, 
	const eiTag job, 
	const eiThreadID threadId)
{
}

static void rprocess_job_finished(
	eiProcess *process, 
	const eiTag job, 
	const eiInt job_state, 
	const eiThreadID threadId)
{
}

static void rprocess_info(
	eiProcess *process, 
	const char *text)
{
}

struct RenderProcess
{
	eiProcess	base;
	eiTag		scene_tag;

	RenderProcess()
	{
		base.pass_started = rprocess_pass_started;
		base.pass_finished = rprocess_pass_finished;
		base.job_started = rprocess_job_started;
		base.job_finished = rprocess_job_finished;
		base.info = rprocess_info;
		scene_tag = EI_NULL_TAG;
	}
};

static eiUint custom_trace(
	eiProcess *process, 
	eiTLS *tls, 
	eiBaseBucket *bucket, 
	eiTag scene_tag)
{
	RenderProcess *rp = (RenderProcess *)process;
	rp->scene_tag = scene_tag;
	return 0;
}

bool loadESS(const char * path)
{
	ei_info("Loading ESS file %s...\n", path);

	ei_context();

	if (!ei_parse2(path, EI_TRUE))
	{
		ei_error("Cannot open file %s\n", path);

		ei_end_context();
		return false;
	}

	// Get camera parameters
	eiTag cam_inst_tag = ei_find_node("GlobalCameraInstanceName0x32f24105_0x74e20f38");
	if (cam_inst_tag == EI_NULL_TAG)
	{
		ei_error("No camera instance in scene %s\n", path);

		ei_end_context();
		return false;
	}

	eiDataAccessor<eiNode> cam_inst(cam_inst_tag);
	eiTag cam_tag = ei_node_get_node(cam_inst.get(), ei_node_find_param(cam_inst.get(), "element"));
	if (cam_tag == EI_NULL_TAG)
	{
		ei_error("No camera object in scene %s\n", path);

		ei_end_context();
		return false;
	}

	eiDataAccessor<eiNode> cam(cam_tag);
	eiScalar focal = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "focal"));
	if (focal == EI_MAX_SCALAR)
	{
		ei_error("Unsupported orthographic camera found in scene %s\n", path);

		ei_end_context();
		return false;
	}

	eiScalar aperture = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "aperture"));
	eiScalar aspect = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "aspect"));

	eiScalar fovx = 2.0f * atanf(aperture * 0.5f / focal);
	eiScalar fovy = fovx / aspect;

	eiMatrix cam_to_world = *ei_node_get_matrix(cam_inst.get(), ei_node_find_param(cam_inst.get(), "transform"));

	// Force not to use GI cache, since it will consume 
	// time in ei_render_run_begin called below
	ei_override_enum("options", "engine", "hybrid path tracer");

	// A bit tricky, get tessellated scene using custom trace callback
	RenderProcess rp;
	ei_set_custom_trace(custom_trace);
	ei_job_set_process(&rp.base);

	ei_tessellate_begin(
		"mtoer_instgroup_00", 
		"GlobalCameraInstanceName0x32f24105_0x74e20f38", 
		"GlobalOptionsName0x32f24105_0x74e20f38");

	// We should have got tessellated scene here
	if (rp.scene_tag == EI_NULL_TAG)
	{
		ei_error("Failed to tessellate the scene %s\n", path);

		ei_tessellate_end();
		ei_end_context();
		return false;
	}

	// Make a scope to force releasing of object references
	// before calling ei_end_context
	do
	{
		eiTag scene_root_tag = ei_rt_scene_root(rp.scene_tag);
		eiDataAccessor<eiRaySubscene> scene_root(scene_root_tag);

		// Loop over all object instances
		eiDataTableAccessor<eiTag> obj_inst_tags(scene_root->object_instances);
		for (eiInt i = 0; i < obj_inst_tags.size(); ++i)
		{
			eiTag obj_inst_tag = obj_inst_tags.get(i);
			eiDataAccessor<eiRayObjectInstance> obj_inst(obj_inst_tag);
			// We don't support translating volume and procedural objects currently
			if (obj_inst->is_volume_object || obj_inst->is_procedural_object)
			{
				continue;
			}

			// We don't support translating light primitives currently
			eiDataAccessor<eiRayObject> obj(obj_inst->object);
			if (obj->is_light)
			{
				continue;
			}

			// Ignore geometry without material
			eiDataTableAccessor<eiTag> mtl_list(obj_inst->attr.mtl_list);
			eiInt num_mtls = mtl_list.size();
			if (num_mtls <= 0)
			{
				continue;
			}

			eiDataAccessor<eiNode> inst_node(obj_inst->leaf_instance);
			for (eiInt j = 0; j < num_mtls; ++j)
			{
				eiTag mtl_tag = mtl_list.get(j);
				eiDataAccessor<eiNode> mtl(mtl_tag);
				eiTag surface_shader_tag = ei_node_get_node(mtl.get(), 
					ei_node_find_param(mtl.get(), "surface_shader"));
				eiDataAccessor<eiNode> surface_shader(surface_shader_tag);
			}

			eiMatrix xform = obj_inst->flatten_object_to_world;
			eiMatrix normal_xform = transpose(obj_inst->flatten_motion_world_to_object);
			eiBool negative_scale = neg_parity(xform);

			eiDataAccessor<eiNode> obj_node(obj->source);
			eiIndex uv_channel_offset = EI_NULL_INDEX;
			if (obj_node->uv_index != EI_NULL_INDEX)
			{
				eiNodeParam *uv_param = ei_node_read_param(obj_node.get(), obj_node->uv_index);
				uv_channel_offset = uv_param->channel_offset;
			}

			// Get material by material index
			eiTag mtl_index_list_tag = EI_NULL_TAG;
			if (obj_node->mtl_index_index != EI_NULL_INDEX)
			{
				mtl_index_list_tag = ei_node_get_array(obj_node.get(), obj_node->mtl_index_index);
			}
			eiDataTableAccessor<eiIndex> mtl_index_list(mtl_index_list_tag);

			// Get triangulation for subdivision mesh
			eiTag triangulation_tag = ei_node_get_array(obj_node.get(), 
				ei_node_find_param(obj_node.get(), "triangulation"));
			eiDataTableAccessor<eiIndex> triangulation(triangulation_tag);

			// Loop over all tessellations
			eiDataTableAccessor<eiTag> tessel_tags(obj->tessels);
			for (eiInt j = 0; j < tessel_tags.size(); ++j)
			{
				eiTag tessel_tag = tessel_tags.get(j);
				eiDataAccessor<eiRayTessel> tessel(tessel_tag);
				// Ignore tessellations without normal vectors or with 
				// compressed normals for now
				if (tessel->normal_channel_tag == EI_NULL_TAG || 
					tessel->normal_channel_type != EI_TYPE_VECTOR)
				{
					continue;
				}

				eiDataTableAccessor<eiVector> pos_list(tessel->pos_list);
				eiDataTableAccessor<eiIndex> tri_list(tessel->tri_list);
				eiDataTableAccessor<eiVector> normal_list(tessel->normal_channel_tag);
				eiDataTableAccessor<eiIndex> normal_idx_list(tessel->normal_channel_indices);
				eiTag uv_tag = EI_NULL_TAG;
				eiTag uv_idx_tag = EI_NULL_TAG;
				if (uv_channel_offset != EI_NULL_INDEX)
				{
					ei_rt_tessel_get_fvar_channel(tessel.get(), uv_channel_offset, uv_tag, uv_idx_tag);
				}
				eiDataTableAccessor<eiVector> uv_list(uv_tag);
				eiDataTableAccessor<eiIndex> uv_idx_list(uv_idx_tag);

				// Loop over all triangles
				eiInt tri_count = tri_list.size() / 3;
				for (eiInt k = 0; k < tri_count; ++k)
				{
					eiIndex vi0 = tri_list.get(k * 3 + 0);
					eiIndex vi1 = tri_list.get(k * 3 + 1);
					eiIndex vi2 = tri_list.get(k * 3 + 2);
					eiVector v0 = point_transform(pos_list.get(vi0), xform);
					eiVector v1 = point_transform(pos_list.get(vi1), xform);
					eiVector v2 = point_transform(pos_list.get(vi2), xform);

					eiIndex ni0 = vi0;
					eiIndex ni1 = vi1;
					eiIndex ni2 = vi2;
					if (tessel->normal_channel_indices != EI_NULL_TAG)
					{
						ni0 = normal_idx_list.get(k * 3 + 0);
						ni1 = normal_idx_list.get(k * 3 + 1);
						ni2 = normal_idx_list.get(k * 3 + 2);
					}
					eiVector n0 = vector_transform(normal_list.get(ni0), normal_xform);
					eiVector n1 = vector_transform(normal_list.get(ni1), normal_xform);
					eiVector n2 = vector_transform(normal_list.get(ni2), normal_xform);

					eiVector uv0 = ei_vector(0.0f);
					eiVector uv1 = ei_vector(0.0f);
					eiVector uv2 = ei_vector(0.0f);
					if (uv_channel_offset != EI_NULL_INDEX)
					{
						eiIndex uvi0 = vi0;
						eiIndex uvi1 = vi1;
						eiIndex uvi2 = vi2;
						if (uv_idx_tag != EI_NULL_TAG)
						{
							uvi0 = uv_idx_list.get(k * 3 + 0);
							uvi1 = uv_idx_list.get(k * 3 + 1);
							uvi2 = uv_idx_list.get(k * 3 + 2);
						}
						uv0 = uv_list.get(uvi0);
						uv1 = uv_list.get(uvi1);
						uv2 = uv_list.get(uvi2);
					}

					eiIndex prim_index = RAY_TRI_PRIM_INDEX(
						ei_rt_tessel_get_triangles(tessel.get()) + k);
					if (triangulation_tag != EI_NULL_TAG)
					{
						prim_index = triangulation.get(prim_index);
					}
					eiIndex mtl_index = 0;
					if (mtl_index_list_tag != EI_NULL_TAG)
					{
						mtl_index = mtl_index_list.get(prim_index) % ((eiIndex)num_mtls);
					}
					eiTag mtl_tag = mtl_list.get(mtl_index);
				}
			}
		}
	}
	while (0);

	ei_tessellate_end();

	ei_set_custom_trace(NULL);
	ei_job_set_process(NULL);

	ei_end_context();
	return true;
}

int main(int argc, char* argv[])
{
	return 0;
}
