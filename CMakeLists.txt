cmake_minimum_required (VERSION 2.6)

# Create project
#
project (elaratoon)

# Set rpath for Linux
#
set (CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set (CMAKE_INSTALL_RPATH "$ORIGIN")

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy (SET CMP0026 OLD)
list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include (CreateLaunchers)

# Add external includes
#
include_directories (
	external/elara_sdk/include/ 
	external/trimesh2/include/ 
	.
)

# Add dependencies
set (ALL_LIBS 
	${CMAKE_SOURCE_DIR}/external/elara_sdk/lib/liber.lib 
)

# Build static
#
add_definitions(
	-D_CRT_SECURE_NO_WARNINGS 
)

# Find source files
#
file (GLOB h_src *.h)
file (GLOB cpp_src *.cpp)
set (trimesh2_src 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/conn_comps.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/diffuse.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/edgeflip.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/faceflip.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/filter.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/ICP.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/KDtree.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/lmsmooth.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/overlap.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/remove.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/reorder_verts.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/shared.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/subdiv.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_bounding.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_connectivity.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_curvature.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_grid.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_io.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_normals.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_pointareas.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_stats.cc 
	${CMAKE_SOURCE_DIR}/external/trimesh2/libsrc/TriMesh_tstrips.cc 
)

# Make Source groups
#
source_group ("trimesh2" FILES ${trimesh2_src})

# Main executable
#
add_executable (elaratoon 
	${h_src} 
	${cpp_src} 
	${trimesh2_src} 
	README.md
)
target_link_libraries (elaratoon ${ALL_LIBS})

# Xcode and Visual working directories
#
set_target_properties (elaratoon 
	PROPERTIES 
	XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR 
	"${CMAKE_CURRENT_SOURCE_DIR}"
)
create_target_launcher (elaratoon 
	WORKING_DIRECTORY 
	"${CMAKE_CURRENT_SOURCE_DIR}"
)
add_custom_command(
   TARGET elaratoon POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/external/elara_sdk/shaders" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/shaders"
)
