
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <list>

// Include Elara SDK
#include <ei.h>
#include <ei_dataflowx.h>

#include "ess_loader.h"

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

	eiTag cam_tag = EI_NULL_TAG;
	if (!loadESS(
		scene_filename, 
		mesh_list, 
		cam_tag))
	{
		return -1;
	}

	if (mesh_list.empty())
	{
		ei_error("Cannot find mesh to render.\n");
		return -1;
	}

	if (cam_tag == EI_NULL_TAG)
	{
		ei_error("Cannot find camera to render.\n");
		return -1;
	}

	eiDataAccessor<eiNode> cam(cam_tag);

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
		return -1;
	}

	fprintf(file, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fprintf(file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fprintf(file, "<svg width=\"100%%\" height=\"100%%\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");

	eiInt num_chains = (eiInt)contourChainGroup.contourChainGroup.size();
	eiInt chain_index = 0;
	for (std::list<ContourChain *>::iterator chain_iter = contourChainGroup.contourChainGroup.begin(); 
		chain_iter != contourChainGroup.contourChainGroup.end(); ++ chain_iter, ++ chain_index)
	{
		ContourChain *chain = *chain_iter;
		eiInt chain_color = lfloorf(((eiScalar)(chain_index + 1) / (eiScalar)(num_chains + 1)) * 16777215.0f);

		fprintf(file, "<polyline points=\"");

		bool is_first_point = true;
		for (std::list<ContourPoint>::iterator point_iter = chain->contourChain.begin(); 
			point_iter != chain->contourChain.end(); ++ point_iter)
		{
			ContourPoint & point = *point_iter;

			if (!is_first_point)
			{
				fprintf(file, ",");
			}

			eiVector raster;
			if (ei_std_camera_object_to_screen(
				cam.get(), 
				&raster, 
				&point.pos, 
				&g_IdentityMatrix))
			{
				fprintf(file, "%f %f", raster.x, raster.y);
				is_first_point = false;
			}
		}

		fprintf(file, "\" style=\"fill:none;stroke:#%X;stroke-width:0.25\"/>\n", chain_color);
	}

	fprintf(file, "</svg>\n");
	fclose(file);

	ei_info("Number of contour chains: %d\n", num_chains);

	// Elara's database is kept open for camera projection
	ei_tessellate_end();

	ei_set_custom_trace(NULL);
	ei_job_set_process(NULL);

	ei_end_context();

	ei_info("Scene removed.\n");

	getch();
	
	return 0;
}
