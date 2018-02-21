
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <list>

// Include Elara SDK
#include <ei.h>
#include <ei_dataflowx.h>
#include <ei_raytracer.h>

#include "ess_loader.h"

static bool toonCallback(void *param, eiScalar percent)
{
	return true;
}

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

	if (!loadESS(scene_filename, toonCallback, NULL))
	{
		return -1;
	}

	getch();
	
	return 0;
}
