
#pragma once

// Include standard headers
#include <vector>

// Include Elara SDK
#include <ei.h>

// Include trimesh headers
#include <TriMesh.h>

trimesh::TriMesh *create_trimesh(
	std::vector<eiVector> & in_vertices, 
	std::vector<eiVector2> & in_uvs, 
	std::vector<eiVector> & in_normals);
