
#include "ess_loader.h"
#include "trimesh_builder.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>

// Include Elara SDK
#include <ei_raytracer.h>
#include <ei_data_table.h>

#define SQR(x)	((x) * (x))

struct ContourPoint
{
	eiVector	pos;
	eiScalar	scale;
};

struct ContourChain
{
	ContourChain()
	{
		forceClosed = false;
	}

	~ContourChain()
	{
	}

	bool addSegmentToChain(const ContourPoint & v1, const ContourPoint & v2)
	{
		if (contourChain.empty()) {
			contourChain.push_back(v1);
			contourChain.push_back(v2);
			return true;
		}
		else {
			ContourPoint & start = contourChain.front();
			ContourPoint & end = contourChain.back();

			if (isCloseEnough(v1, start)) {
				contourChain.push_front(v2);
				return true;
			}
			else if (isCloseEnough(v2, start)) {
				contourChain.push_front(v1);
				return true;
			}
			else if (isCloseEnough(v1, end)) {
				contourChain.push_back(v2);
				return true;
			}
			else if (isCloseEnough(v2, end)) {
				contourChain.push_back(v1);
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool isChainClosed()
	{
		if (forceClosed) {
			return true;
		}

		if (!contourChain.empty() && 
			isCloseEnough(contourChain.front(), contourChain.back())) {
			return true;
		} else {
			return false;
		}
	}

	void forceChainClosed()
	{
		forceClosed = true;
	}

	bool isCloseEnough(const ContourPoint & v1, const ContourPoint & v2)
	{
		if (lensq(v1.pos - v2.pos) < max(v1.scale, v2.scale)) {
			return true;
		} else {
			return false;
		}
	}

	bool forceClosed;
	std::list<ContourPoint> contourChain;
};

struct ContourChainGroup
{
	ContourChainGroup()
	{
	}

	~ContourChainGroup()
	{
		resetGroup();
	}

	void addSegmentToGroup(const eiVector & p1, const eiVector & p2)
	{
		const eiScalar seg_scale = lensq(p1 - p2) * 0.0001f;
		ContourPoint v1, v2;
		v1.pos = p1;
		v1.scale = seg_scale;
		v2.pos = p2;
		v2.scale = seg_scale;

		bool segmentAdded = false;

		for (std::list<ContourChain *>::iterator cit = contourChainGroup.begin(); 
			cit != contourChainGroup.end(); ++cit) {
			ContourChain *eachChain = *cit;

			if (eachChain->isChainClosed()) {
				continue;
			}

			if (eachChain->addSegmentToChain(v1, v2)) {
				segmentAdded = true;
				break;
			}
		}

		if (!segmentAdded) {
			ContourChain *newChain = new ContourChain();
			newChain->addSegmentToChain(v1, v2);
			contourChainGroup.push_front(newChain);
		}
	}

	void resetGroup()
	{
		for (std::list<ContourChain *>::iterator cit = contourChainGroup.begin(); 
			cit != contourChainGroup.end(); ++cit) {
			ContourChain *eachChain = *cit;
			delete eachChain;
		}

		contourChainGroup.clear();
	}

	void finishedAdding()
	{
		// there are no more pairs coming... so chains that aren't yet closed 
		// are never going to be, so force-close them
		for (std::list<ContourChain *>::iterator it = contourChainGroup.begin(); 
			it != contourChainGroup.end(); ++it) {
			ContourChain *eachChain = *it;
			if (!eachChain->isChainClosed()) {
				eachChain->forceChainClosed();
			}
		}
	}

	std::list<ContourChain *> contourChainGroup;
};

inline eiVector interpolateZeroPoint(
	eiVector v1, eiScalar val1, 
	eiVector v2, eiScalar val2)
{
	if (val2 < val1) {
		eiVector temp = v1;
		eiScalar tempval = val1;
		v1 = v2;
		v2 = temp;
		val1 = val2;
		val2 = tempval;
	}

	eiScalar t = -val1 / (val2 - val1);
	eiVector returnVec = v1 * (1.0f - t) + v2 * t;
	return returnVec;
}

void determineFaceVisibilities(
	trimesh::TriMesh *m, 
	std::vector<eiInt> & faceVisible)
{
	faceVisible.resize(m->faces.size());

	for (eiInt i = 0; i < m->faces.size(); ++i) {
		eiVector v1 = ei_vector(m->vertices[m->faces[i][0]][0], m->vertices[m->faces[i][0]][1], m->vertices[m->faces[i][0]][2]);
		eiVector v2 = ei_vector(m->vertices[m->faces[i][1]][0], m->vertices[m->faces[i][1]][1], m->vertices[m->faces[i][1]][2]);
		eiVector v3 = ei_vector(m->vertices[m->faces[i][2]][0], m->vertices[m->faces[i][2]][1], m->vertices[m->faces[i][2]][2]);
		eiVector n1 = ei_vector(m->normals[m->faces[i][0]][0], m->normals[m->faces[i][0]][1], m->normals[m->faces[i][0]][2]);
		eiVector n2 = ei_vector(m->normals[m->faces[i][1]][0], m->normals[m->faces[i][1]][1], m->normals[m->faces[i][1]][2]);
		eiVector n3 = ei_vector(m->normals[m->faces[i][2]][0], m->normals[m->faces[i][2]][1], m->normals[m->faces[i][2]][2]);
		
		// perspective: where v(p) = c - p
		eiVector t_v1 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v1);
		eiVector t_v2 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v2);
		eiVector t_v3 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v3);

		eiVector t_n1 = normalize(n1);
		eiVector t_n2 = normalize(n2);
		eiVector t_n3 = normalize(n3);

		eiScalar dot1 = dot(t_v1, t_n1);
		eiScalar dot2 = dot(t_v2, t_n2);
		eiScalar dot3 = dot(t_v3, t_n3);

		if (dot1 >= 0.0f && dot2 >= 0.0f && dot3 >= 0.0f) {
			faceVisible[i] = 0;
		} else if (dot1 < 0.0f && dot2 < 0.0f && dot3 < 0.0f) {
			faceVisible[i] = 2;
		} else {
			faceVisible[i] = 1;
		}
	}
}

void drawSmoothContoursFunc(
	trimesh::TriMesh *m, 
	const std::vector<eiInt> & faceVisible, 
	ContourChainGroup & contourChainGroup)
{
	std::vector<bool> faceProcessed(m->faces.size());
	std::list<eiInt> faceToProcess;
	for (eiInt i = 0; i < m->faces.size(); ++i) {
		if (faceVisible[i] == 1) {
			faceProcessed[i] = false;
			faceToProcess.push_back(i);
		}
		else {
			faceProcessed[i] = true;
		}
	}

	while (!faceToProcess.empty()) {
		eiInt faceIndex = faceToProcess.front();
		faceToProcess.pop_front();	

		if (faceProcessed[faceIndex]) {
			continue;
		}

		eiVector v1 = ei_vector(m->vertices[m->faces[faceIndex][0]][0], m->vertices[m->faces[faceIndex][0]][1], m->vertices[m->faces[faceIndex][0]][2]);
		eiVector v2 = ei_vector(m->vertices[m->faces[faceIndex][1]][0], m->vertices[m->faces[faceIndex][1]][1], m->vertices[m->faces[faceIndex][1]][2]);
		eiVector v3 = ei_vector(m->vertices[m->faces[faceIndex][2]][0], m->vertices[m->faces[faceIndex][2]][1], m->vertices[m->faces[faceIndex][2]][2]);
		eiVector n1 = ei_vector(m->normals[m->faces[faceIndex][0]][0], m->normals[m->faces[faceIndex][0]][1], m->normals[m->faces[faceIndex][0]][2]);
		eiVector n2 = ei_vector(m->normals[m->faces[faceIndex][1]][0], m->normals[m->faces[faceIndex][1]][1], m->normals[m->faces[faceIndex][1]][2]);
		eiVector n3 = ei_vector(m->normals[m->faces[faceIndex][2]][0], m->normals[m->faces[faceIndex][2]][1], m->normals[m->faces[faceIndex][2]][2]);

		// perspective: where v(p) = c - p
		eiVector t_v1 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v1);
		eiVector t_v2 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v2);
		eiVector t_v3 = normalize(ei_vector(0.0f, 0.0f, 0.0f) - v3);

		eiVector t_n1 = normalize(n1);
		eiVector t_n2 = normalize(n2);
		eiVector t_n3 = normalize(n3);

		eiScalar dot1 = dot(t_v1, t_n1);
		eiScalar dot2 = dot(t_v2, t_n2);
		eiScalar dot3 = dot(t_v3, t_n3);

		// draw mesh edge contour (in red, if needed) 2 out of 3 are 1 sign, 1's the other
		eiVector p1;
		eiVector p2;
		bool drawStroke = false;

		if (dot1 >= 0.0f && dot2 >= 0.0f && dot3 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot1 < 0.0f && dot2 < 0.0f && dot3 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot2 >= 0.0f && dot3 >= 0.0f && dot1 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = true;
		}
		else if (dot2 < 0.0f && dot3 < 0.0f && dot1 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = true;
		}
		else if (dot3 >= 0.0f && dot1 >= 0.0f && dot2 < 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot3 < 0.0f && dot1 < 0.0f && dot2 >= 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}

		if (drawStroke) {
			contourChainGroup.addSegmentToGroup(p1, p2);

			// resort remainder of list such that there is a better ordering (for contour chaining step)		
			for (eiInt j = 0; j < 3; ++j) {
				eiInt adjacentFaceIndex = m->across_edge[faceIndex][j];
				if (adjacentFaceIndex >= 0 && !faceProcessed[adjacentFaceIndex]) {
					faceToProcess.push_front(adjacentFaceIndex);
				}
			}
		}

		faceProcessed[faceIndex] = true;
	}
}

void calculateKappaRFunc(
	trimesh::TriMesh *m, 
	std::vector<eiScalar> & kappa_r)
{
	kappa_r.resize(m->vertices.size());

	for (eiInt i = 0; i < m->vertices.size(); ++i) {
		eiVector eachVertex = ei_vector(m->vertices[i][0], m->vertices[i][1], m->vertices[i][2]);
		
		eiVector viewVec = normalize(ei_vector(0.0f, 0.0f, 0.0f) - eachVertex);

		// here, u and v are components of view vector, u along principal direction, 
		// v in direction perpendicular to that on tangent plane. 
		// this corresponds to the "cos(phi)" defined in the paper.
		eiScalar u = dot(viewVec, ei_vector(m->pdir1[i][0], m->pdir1[i][1], m->pdir1[i][2]));
		eiScalar v = dot(viewVec, ei_vector(m->pdir2[i][0], m->pdir2[i][1], m->pdir2[i][2]));
		
		kappa_r[i] = (m->curv1[i] * u * u + m->curv2[i] * v * v) / (u * u + v * v);
	}
}

void calculateGradKappaRFunc(
	trimesh::TriMesh *m, 
	std::vector<eiScalar> & dwk_r)
{
	dwk_r.resize(m->vertices.size());

	for (eiInt i = 0; i < m->vertices.size(); ++i) {
		eiVector eachVertex = ei_vector(m->vertices[i][0], m->vertices[i][1], m->vertices[i][2]);
		
		eiVector viewVec = normalize(ei_vector(0.0f, 0.0f, 0.0f) - eachVertex);

		eiVector eachNormal = normalize(ei_vector(m->normals[i][0], m->normals[i][1], m->normals[i][2]));
		eiScalar ndotv = dot(viewVec, eachNormal);

		eiScalar u = dot(viewVec, ei_vector(m->pdir1[i][0], m->pdir1[i][1], m->pdir1[i][2]));
		eiScalar v = dot(viewVec, ei_vector(m->pdir2[i][0], m->pdir2[i][1], m->pdir2[i][2]));
		
		dwk_r[i] = (u * u * (u * m->dcurv[i][0] + 3.0f * v * m->dcurv[i][1]) + 
			v * v * (3.0f * u * m->dcurv[i][2] + v * m->dcurv[i][3])) / (u * u + v * v) 
	    	- 2.0f * ndotv * SQR((m->curv2[i] - m->curv1[i]) * u * v / (u * u + v * v));
	}
}

void drawVisibleSuggestiveContoursFunc(
	trimesh::TriMesh *m, 
	const std::vector<eiInt> & faceVisible, 
	const std::vector<eiScalar> & kappa_r, 
	const std::vector<eiScalar> & dwk_r, 
	bool testDWKRPositive, 
	eiScalar thetaC, 
	eiScalar t_d, 
	ContourChainGroup & contourChainGroup)
{
	// so now we got the curvatures, what do we do with them.
	// we find the zero crossings of the DERIVATIVE of the kappa_r's with 
	// respect to w, and draw lines between them.
	// for this, we can just use the algorithm from before, just instead 
	// of the dot product of n and v.
	// its the zero crossing of dkappa_r/dw thing.

	// we go through each face for this part
	std::vector<bool> faceProcessed(m->faces.size());
	std::list<eiInt> faceToProcess;

	for (eiInt i = 0; i < m->faces.size(); ++i) {
		if (faceVisible[i] < 2) {
			if (!((kappa_r[m->faces[i][0]] > 0.0f && kappa_r[m->faces[i][1]] > 0.0f && kappa_r[m->faces[i][2]] > 0.0f) 
				|| (kappa_r[m->faces[i][0]] < 0.0f && kappa_r[m->faces[i][1]] < 0.0f && kappa_r[m->faces[i][2]] < 0.0f))) {
				faceProcessed[i] = false;
				faceToProcess.push_back(i);
			}
			else
				faceProcessed[i] = true;
		}
		else {
			faceProcessed[i] = true;
		}
	}

	while (!faceToProcess.empty()) {
		eiInt faceIndex = faceToProcess.front();
		faceToProcess.pop_front();

		if (faceProcessed[faceIndex]) {
			continue;
		}

		eiVector v1 = ei_vector(m->vertices[m->faces[faceIndex][0]][0], m->vertices[m->faces[faceIndex][0]][1], m->vertices[m->faces[faceIndex][0]][2]);
		eiVector v2 = ei_vector(m->vertices[m->faces[faceIndex][1]][0], m->vertices[m->faces[faceIndex][1]][1], m->vertices[m->faces[faceIndex][1]][2]);
		eiVector v3 = ei_vector(m->vertices[m->faces[faceIndex][2]][0], m->vertices[m->faces[faceIndex][2]][1], m->vertices[m->faces[faceIndex][2]][2]);

		eiScalar dot1 = kappa_r[m->faces[faceIndex][0]];
		eiScalar dot2 = kappa_r[m->faces[faceIndex][1]];
		eiScalar dot3 = kappa_r[m->faces[faceIndex][2]];

		// draw suggestive contour (in red, if needed) 2 out of 3 are 1 sign, 1's the other 
		// HERE WE CHECK TO MAKE SURE THAT D_w * k_r > 0
		// or another way to put it is that grad(k_r) * w > 0 for each of the vertices.
		// in fact, we check t_d < grad(k_r) DOT w / || w ||
		bool allArePositive = true;

		if (testDWKRPositive) {
			for (eiInt j = 0; j < 3; ++j) {
				eiVector eachVertex = ei_vector(m->vertices[m->faces[faceIndex][j]][0], m->vertices[m->faces[faceIndex][j]][1], m->vertices[m->faces[faceIndex][j]][2]);
				eiVector eachNormal = ei_vector(m->normals[m->faces[faceIndex][j]][0], m->normals[m->faces[faceIndex][j]][1], m->normals[m->faces[faceIndex][j]][2]);

				eiVector viewVec = ei_vector(0.0f, 0.0f, 0.0f) - eachVertex;
				eiVector normalProj = eachNormal;
				normalProj = normalize(normalProj) * dot(normalProj, viewVec);
				eiVector w = viewVec - normalProj;

				if (dwk_r[m->faces[faceIndex][j]] / len(w) < t_d) {
					allArePositive = false;
					break;
				}
			}
		}

		if (!allArePositive) {
			faceProcessed[faceIndex] = true;
			continue;
		}

		// HERE WE CHECK THAT theta_c < n(p) dot v(p) / || v(p) ||
		bool thetaCIsLess = true;

		for (eiInt j = 0; j < 3; ++j) {
			eiVector eachVertex = ei_vector(m->vertices[m->faces[faceIndex][j]][0], m->vertices[m->faces[faceIndex][j]][1], m->vertices[m->faces[faceIndex][j]][2]);
			eiVector eachNormal = ei_vector(m->normals[m->faces[faceIndex][j]][0], m->normals[m->faces[faceIndex][j]][1], m->normals[m->faces[faceIndex][j]][2]);

			eiVector viewVec = ei_vector(0.0f, 0.0f, 0.0f) - eachVertex;
			eiVector eachNormalTransformed = eachNormal;

			if (!(thetaC < acosf(dot(eachNormalTransformed, viewVec) / len(viewVec)))) {
				thetaCIsLess = false;
				break;
			}
		}

		if (!thetaCIsLess) {
			faceProcessed[faceIndex] = true;
			continue;
		}
	
		// these have to pruned according to the formula D_w*k(r) > 0
		eiVector p1;
		eiVector p2;	
		bool drawStroke = false;

		if (dot1 >= 0.0f && dot2 >= 0.0f && dot3 < 0.0f) {				
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot1 < 0.0f && dot2 < 0.0f && dot3 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot2 >= 0.0f && dot3 >= 0.0f && dot1 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = true;
		}
		else if (dot2 < 0.0f && dot3 < 0.0f && dot1 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = true;
		}
		else if (dot3 >= 0.0f && dot1 >= 0.0f && dot2 < 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}
		else if (dot3 < 0.0f && dot1 < 0.0f && dot2 >= 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = true;
		}

		// draw stroke
		if (drawStroke) {
			contourChainGroup.addSegmentToGroup(p1, p2);			

			// if a stroke is drawn, do the reordering to prioritize neighbours for contour chaining			
			for (eiInt j = 0; j < 3; ++j) {
				eiInt adjacentFaceIndex = m->across_edge[faceIndex][j];
				if (adjacentFaceIndex >= 0 && !faceProcessed[adjacentFaceIndex]) {
					faceToProcess.push_front(adjacentFaceIndex);
				}
			}
		}

		faceProcessed[faceIndex] = true;
	}
}

void flushCurve(std::vector<eiVector> & polyline, FILE *file, eiInt chain_color)
{
	if (polyline.empty()) {
		return;
	}

	fprintf(file, "<polyline points=\"");

	for (size_t i = 0; i < polyline.size(); ++i)
	{
		if (i != 0) {
			fprintf(file, ",");
		}

		fprintf(file, "%f %f", polyline[i].x, polyline[i].y);
	}

	fprintf(file, "\" style=\"fill:none;stroke:#%X;stroke-width:1.0\"/>\n", chain_color);

	polyline.resize(0);
}

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

	RenderProcess()
	{
		base.pass_started = rprocess_pass_started;
		base.pass_finished = rprocess_pass_finished;
		base.job_started = rprocess_job_started;
		base.job_finished = rprocess_job_finished;
		base.info = rprocess_info;
	}
};

static eiUint custom_trace(
	eiProcess *process, 
	eiTLS *tls, 
	eiBaseBucket *bucket, 
	eiTag scene_tag)
{
	RenderProcess *rp = (RenderProcess *)process;

	// We should have got tessellated scene here
	if (scene_tag == EI_NULL_TAG)
	{
		ei_error("Failed to tessellate the scene.\n");
		return 0;
	}

	std::vector<trimesh::TriMesh *> mesh_list;

	// Get camera parameters
	eiTag cam_inst_tag = ei_find_node("GlobalCameraInstanceName0x32f24105_0x74e20f38");
	if (cam_inst_tag == EI_NULL_TAG)
	{
		ei_error("No camera instance in scene.\n");
		return 0;
	}

	eiDataAccessor<eiNode> cam_inst(cam_inst_tag);
	eiTag cam_tag = ei_node_get_node(cam_inst.get(), ei_node_find_param(cam_inst.get(), "element"));
	if (cam_tag == EI_NULL_TAG)
	{
		ei_error("No camera object in scene.\n");
		return 0;
	}

	eiDataAccessor<eiNode> cam(cam_tag);
	eiScalar focal = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "focal"));
	if (focal == EI_MAX_SCALAR)
	{
		ei_error("Unsupported orthographic camera found in scene.\n");
		return 0;
	}

	eiScalar aperture = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "aperture"));
	eiScalar aspect = ei_node_get_scalar(cam.get(), ei_node_find_param(cam.get(), "aspect"));

	eiScalar fovx = 2.0f * atanf(aperture * 0.5f / focal);
	eiScalar fovy = fovx / aspect;

	eiMatrix cam_to_world = *ei_node_get_matrix(cam_inst.get(), ei_node_find_param(cam_inst.get(), "transform"));
	eiMatrix world_to_cam = inverse(cam_to_world);
	
	eiTag scene_root_tag = ei_rt_scene_root(scene_tag);
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

		// Local cache of triangle mesh for current instance
		std::vector<eiVector> out_vertices;
		std::vector<eiVector2> out_uvs;
		std::vector<eiVector> out_normals;

		eiDataAccessor<eiNode> inst_node(obj_inst->leaf_instance);
		for (eiInt j = 0; j < num_mtls; ++j)
		{
			eiTag mtl_tag = mtl_list.get(j);
			eiDataAccessor<eiNode> mtl(mtl_tag);
			eiTag surface_shader_tag = ei_node_get_node(mtl.get(), 
				ei_node_find_param(mtl.get(), "surface_shader"));
			eiDataAccessor<eiNode> surface_shader(surface_shader_tag);
		}

		// Transform all data into camera space as required by contour shading
		eiMatrix xform = obj_inst->flatten_object_to_world * world_to_cam;
		eiMatrix normal_xform = transpose(inverse(xform));
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

				// Add mesh data to local cache
				if (!negative_scale)
				{
					out_vertices.push_back(v0);
					out_vertices.push_back(v1);
					out_vertices.push_back(v2);

					out_normals.push_back(n0);
					out_normals.push_back(n1);
					out_normals.push_back(n2);

					out_uvs.push_back(ei_vector2(uv0.x, uv0.y));
					out_uvs.push_back(ei_vector2(uv1.x, uv1.y));
					out_uvs.push_back(ei_vector2(uv2.x, uv2.y));
				}
				else
				{
					out_vertices.push_back(v2);
					out_vertices.push_back(v1);
					out_vertices.push_back(v0);

					out_normals.push_back(n2);
					out_normals.push_back(n1);
					out_normals.push_back(n0);

					out_uvs.push_back(ei_vector2(uv2.x, uv2.y));
					out_uvs.push_back(ei_vector2(uv1.x, uv1.y));
					out_uvs.push_back(ei_vector2(uv0.x, uv0.y));
				}
			}
		}

		// Build trimesh from local cache
		trimesh::TriMesh *mesh = create_trimesh(
			out_vertices, 
			out_uvs, 
			out_normals);

		if (mesh == NULL)
		{
			ei_error("Failed to build trimesh for instance %d\n", i);
		}
		else
		{
			// Add mesh for contour shading
			mesh_list.push_back(mesh);
		}
	}

	if (mesh_list.empty())
	{
		ei_error("Cannot find mesh to render.\n");
		return 0;
	}

	ContourChainGroup contourChainGroup;
	contourChainGroup.resetGroup();

	for (size_t i = 0; i < mesh_list.size(); ++i)
	{
		trimesh::TriMesh *m = mesh_list[i];

		std::vector<eiInt> faceVisible;
		determineFaceVisibilities(
			m, 
			faceVisible);

		drawSmoothContoursFunc(
			m, 
			faceVisible, 
			contourChainGroup);

		std::vector<eiScalar> kappa_r;
		calculateKappaRFunc(
			m, 
			kappa_r);

		std::vector<eiScalar> dwk_r;
		calculateGradKappaRFunc(
			m, 
			dwk_r);

		drawVisibleSuggestiveContoursFunc(
			m, 
			faceVisible, 
			kappa_r, 
			dwk_r, 
			true, 
			0.0f, 
			0.0f, 
			contourChainGroup);
	}

	contourChainGroup.finishedAdding();

	// write out SVG file
	FILE *file = fopen("D:/test.svg", "wb");
	if (file == NULL)
	{
		ei_error("Cannot open file for writing SVG.\n");
		return 0;
	}

	fprintf(file, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fprintf(file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fprintf(file, "<svg width=\"100%%\" height=\"100%%\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");


	eiRayTLS *ray_tls = (eiRayTLS *)ei_tls_get_interface(tls, EI_TLS_TYPE_RAYTRACER);
	eiVector cpos = point_transform(ei_vector(0.0f, 0.0f, 0.0f), cam_to_world);
	eiUint num_probe_rays = 0;

	std::vector<eiVector> polyline;
	eiInt num_chains = (eiInt)contourChainGroup.contourChainGroup.size();
	eiInt chain_index = 0;
	for (std::list<ContourChain *>::iterator chain_iter = contourChainGroup.contourChainGroup.begin(); 
		chain_iter != contourChainGroup.contourChainGroup.end(); ++ chain_iter, ++ chain_index)
	{
		ContourChain *chain = *chain_iter;
		eiInt chain_color = lfloorf(((eiScalar)(chain_index + 1) / (eiScalar)(num_chains + 1)) * 16777215.0f);

		for (std::list<ContourPoint>::iterator point_iter = chain->contourChain.begin(); 
			point_iter != chain->contourChain.end(); ++ point_iter)
		{
			ContourPoint & point = *point_iter;

			eiVector raster;
			if (ei_std_camera_object_to_screen(
				cam.get(), 
				&raster, 
				&point.pos, 
				&g_IdentityMatrix))
			{
				++ num_probe_rays;

				eiVector wpos = point_transform(point.pos, cam_to_world);
				eiVector wdir;
				eiScalar max_dist = normalize_len(wdir, cpos - wpos);

				eiRay probe_ray;
				ei_ray_init(
					&probe_ray, 
					tls, 
					ray_tls, 
					bucket, 
					EI_RAY_PROBE, 
					0.0f, 
					EI_FALSE, /* ignore light primitives */
					max_dist);
				probe_ray.E = wpos - wdir * 0.01f;
				probe_ray.I = wdir;

				eiIntersection probe_isect;
				ei_isect_reset(&probe_isect);
				probe_ray.isect = &probe_isect;

				if (!ei_rt_trace(scene_root_tag, &probe_ray))
				{
					polyline.push_back(raster);
				}
				else
				{
					flushCurve(polyline, file, chain_color);
				}

				ei_ray_exit(&probe_ray);
			}
			else
			{
				flushCurve(polyline, file, chain_color);
			}
		}

		flushCurve(polyline, file, chain_color);
	}

	fprintf(file, "</svg>\n");
	fclose(file);

	ei_info("Number of contour chains: %d\n", num_chains);

	return num_probe_rays;
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
	ei_tessellate_end();

	ei_set_custom_trace(NULL);
	ei_job_set_process(NULL);

	ei_end_context();

	ei_info("Scene removed.\n");

	return true;
}
