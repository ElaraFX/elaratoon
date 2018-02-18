
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include Elara SDK
#include <ei.h>

#include "ess_loader.h"

int main(int argc, char* argv[])
{
	char *scene_filename = "scenes/sss_dragon.ess";
	if (argc < 2 || argv[1] == NULL)
	{
		ei_warning("No valid scene is specified, using default one.\n");
	}
	else
	{
		scene_filename = argv[1];
	}

	std::vector<trimesh::TriMesh *> mesh_list;

	if (!loadESS(
		scene_filename, 
		mesh_list))
	{
		return -1;
	}

	if (mesh_list.empty())
	{
		ei_error("Cannot find mesh to render.\n");
		return -1;
	}
	
	return 0;
}
