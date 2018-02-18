
#include "trimesh_builder.h"

// Include standard headers
#include <string.h>
#include <map>

using namespace trimesh;

struct PackedVertex
{
	eiVector	pos;
	eiVector2	uv;
	eiVector	normal;

	inline bool operator < (const PackedVertex & rhs) const
	{
		return memcmp((void *)this, (void *)&rhs, sizeof(PackedVertex)) > 0;
	};
};

inline bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex, eiUint> & VertexToOutIndex, 
	eiUint & result)
{
	std::map<PackedVertex, eiUint>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	} else {
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<eiVector> & in_vertices, 
	std::vector<eiVector2> & in_uvs, 
	std::vector<eiVector> & in_normals, 
	std::vector<eiUint> & out_indices, 
	std::vector<eiVector> & out_vertices, 
	std::vector<eiVector2> & out_uvs, 
	std::vector<eiVector> & out_normals)
{
	std::map<PackedVertex, eiUint> VertexToOutIndex;

	// For each input vertex
	for (size_t i = 0; i < in_vertices.size(); ++i)
	{
		PackedVertex packed;
		packed.pos = in_vertices[i];
		packed.uv = in_uvs[i];
		packed.normal = in_normals[i];

		// Try to find a similar vertex in out_XXXX
		eiUint index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead
			out_indices.push_back(index);
		} else { // If not, it needs to be added in the output data
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			eiUint newindex = (eiUint)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

TriMesh *create_trimesh(
	std::vector<eiVector> & in_vertices, 
	std::vector<eiVector2> & in_uvs, 
	std::vector<eiVector> & in_normals)
{
	if (in_vertices.empty())
	{
		return NULL;
	}

	TriMesh *m = new TriMesh;

	std::vector<eiVector> out_vertices;
	std::vector<eiVector2> out_uvs;
	std::vector<eiVector> out_normals;
	std::vector<eiUint> out_indices;

	indexVBO(
		in_vertices, 
		in_uvs, 
		in_normals, 
		out_indices, 
		out_vertices, 
		out_uvs, 
		out_normals);

	for (size_t i = 0; i < out_vertices.size(); ++i)
	{
		eiVector & pos = out_vertices[i];
		eiVector & normal = out_normals[i];

		m->vertices.push_back(point(pos.x, pos.y, pos.z));
		m->normals.push_back(vec(normal.x, normal.y, normal.z));
	}

	for (size_t i = 0; i < out_indices.size(); i += 3)
	{
		eiUint i0 = out_indices[i * 3 + 0];
		eiUint i1 = out_indices[i * 3 + 1];
		eiUint i2 = out_indices[i * 3 + 2];

		m->faces.push_back(TriMesh::Face(i0, i1, i2));
	}

	// Convert triangle strips to faces, if necessary
    m->need_faces();        
    m->need_normals();
    m->need_bsphere();
	m->need_curvatures();
	m->need_dcurv();
	m->need_neighbors();
	m->need_adjacentfaces();
	m->need_across_edge();

	return m;
}
