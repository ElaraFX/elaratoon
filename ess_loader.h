
#pragma once

// Include standard headers
#include <vector>

// Include trimesh headers
#include <TriMesh.h>

// Include Elara SDK
#include <ei.h>

typedef bool (*ToonProgressCallback)(void *param, eiScalar percent);

void toonRender(ToonProgressCallback cb, void *param);
bool loadESS(const char * path, ToonProgressCallback cb, void *param);
