
#pragma once

// Include standard headers
#include <vector>

// Include trimesh headers
#include <TriMesh.h>

bool loadESS(
	const char * path, 
	std::vector<trimesh::TriMesh *> & mesh_list);
