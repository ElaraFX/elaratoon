
#pragma once

// Include standard headers
#include <vector>

// Include trimesh headers
#include <TriMesh.h>

// Include Elara SDK
#include <ei.h>

typedef bool (*ToonProgressCallback)(void *param, eiScalar percent);

void toonRender(const char *output_filename, ToonProgressCallback cb, void *param);
bool loadESS(const char * path, const char *output_filename, ToonProgressCallback cb, void *param);
