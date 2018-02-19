
#pragma once

// Include standard headers
#include <vector>

// Include trimesh headers
#include <TriMesh.h>

// Include Elara SDK
#include <ei.h>

bool loadESS(
	const char * path, 
	std::vector<trimesh::TriMesh *> & mesh_list, 
	eiTag & cam_tag, 
	eiTag & scene_root_tag);
