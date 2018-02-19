
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <list>

// Include Elara SDK
#include <ei.h>

#include "ess_loader.h"

#define SQR(x)	((x) * (x))

struct ContourChain
{
	ContourChain()
	{
		forceClosed = EI_FALSE;
	}

	~ContourChain()
	{
	}

	eiBool connectsToChain(const eiVector & v1, const eiVector & v2)
	{
		if (contourChain.empty()) {
			// if it's empty, make this the first segment
			return EI_TRUE;
		}
		else {
			eiVector start = contourChain.front();
			eiVector end = contourChain.back();

			if (isCloseEnough(v1, start) ||
				isCloseEnough(v2, start) ||
				isCloseEnough(v1, end) ||
				isCloseEnough(v2, end)) {
				return EI_TRUE;
			}
		}

		return EI_FALSE;
	}

	void addSegmentToChain(const eiVector & v1, const eiVector & v2)
	{
		if (contourChain.empty()) {
			contourChain.push_back(v1);
			contourChain.push_back(v2);
		}
		else {
			eiVector start = contourChain.front();
			eiVector end = contourChain.back();

			if (isCloseEnough(v1, start)) {
				contourChain.push_front(v2);
			}
			else if (isCloseEnough(v2, start)) {
				contourChain.push_front(v1);
			}
			else if (isCloseEnough(v1, end)) {
				contourChain.push_back(v2);
			}
			else if (isCloseEnough(v2, end)) {
				contourChain.push_back(v1);
			}
			else {
			}
		}
	}

	eiBool isChainClosed()
	{
		if (forceClosed) {
			return EI_TRUE;
		}

		if (!contourChain.empty() && 
			isCloseEnough(contourChain.front(), contourChain.back())) {
			return EI_TRUE;
		} else {
			return EI_FALSE;
		}
	}

	void forceChainClosed()
	{
		forceClosed = EI_TRUE;
	}

	eiBool isCloseEnough(const eiVector & v1, const eiVector & v2)
	{
		if (lensq(v1 - v2) < SQR(0.25f)) {
			return EI_TRUE;
		} else {
			return EI_FALSE;
		}
	}

	eiBool forceClosed;
	std::list<eiVector> contourChain;
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

	void addSegmentToGroup(const eiVector & v1, const eiVector & v2)
	{
		// chains are "done" when they are closed
		eiBool haveOpenChains = EI_FALSE;

		for (std::list<ContourChain *>::iterator it = contourChainGroup.begin(); 
			it != contourChainGroup.end(); ++it) {
			ContourChain *eachChain = *it;
			if (!eachChain->isChainClosed()) {
				haveOpenChains = EI_TRUE;
				break;
			}
		}

		if (!haveOpenChains) {
			ContourChain *newChain = new ContourChain();
			contourChainGroup.push_front(newChain);
		}

		for (std::list<ContourChain *>::iterator cit = contourChainGroup.begin(); 
			cit != contourChainGroup.end(); ++cit) {
			ContourChain *eachChain = *cit;

			if (eachChain->isChainClosed()) {
				continue;
			}

			if (eachChain->connectsToChain(v1, v2)) {
				eachChain->addSegmentToChain(v1, v2);
				break;
			}
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

inline eiVector interpolateZeroPoint(eiVector v1, eiScalar val1, eiVector v2, eiScalar val2)
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
	eiVector returnVec = v1 * (1 - t) + v2 * t;
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
	const std::vector<eiBool> & faceVisible, 
	ContourChainGroup & contourChainGroup)
{
	std::vector<eiBool> faceProcessed(m->faces.size());
	std::list<eiInt> faceToProcess;
	for (eiInt i = 0; i < m->faces.size(); ++i) {
		if (faceVisible[i] == 1) {
			faceProcessed[i] = EI_FALSE;
			faceToProcess.push_back(i);
		}
		else {
			faceProcessed[i] = EI_TRUE;
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
		if (dot1 >= 0.0f && dot2 >= 0.0f && dot3 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
		}
		else if (dot1 < 0.0f && dot2 < 0.0f && dot3 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
		}
		else if (dot2 >= 0.0f && dot3 >= 0.0f && dot1 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
		}
		else if (dot2 < 0.0f && dot3 < 0.0f && dot1 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
		}
		else if (dot3 >= 0.0f && dot1 >= 0.0f && dot2 < 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
		}
		else if (dot3 < 0.0f && dot1 < 0.0f && dot2 >= 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
		}

		contourChainGroup.addSegmentToGroup(p1, p2);

		// resort remainder of list such that there is a better ordering (for contour chaining step)		
		for (eiInt j = 0; j < 3; ++j) {
			eiInt adjacentFaceIndex = m->across_edge[faceIndex][j];
			if (adjacentFaceIndex >= 0 && !faceProcessed[adjacentFaceIndex]) {
				faceToProcess.push_front(adjacentFaceIndex);
			}
		}

		faceProcessed[faceIndex] = EI_TRUE;
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
	const std::vector<eiBool> & faceVisible, 
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
	std::vector<eiBool> faceProcessed(m->faces.size());
	std::list<eiInt> faceToProcess;

	for (eiInt i = 0; i < m->faces.size(); ++i) {
		if (faceVisible[i] < 2) {
			if (!((kappa_r[m->faces[i][0]] > 0.0f && kappa_r[m->faces[i][1]] > 0.0f && kappa_r[m->faces[i][2]] > 0.0f) 
				|| (kappa_r[m->faces[i][0]] < 0.0f && kappa_r[m->faces[i][1]] < 0.0f && kappa_r[m->faces[i][2]] < 0.0f))) {
				faceProcessed[i] = EI_FALSE;
				faceToProcess.push_back(i);
			}
			else
				faceProcessed[i] = EI_TRUE;
		}
		else {
			faceProcessed[i] = EI_TRUE;
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
		eiBool allArePositive = EI_TRUE;

		if (testDWKRPositive) {
			for (eiInt j = 0; j < 3; ++j) {
				eiVector eachVertex = ei_vector(m->vertices[m->faces[faceIndex][j]][0], m->vertices[m->faces[faceIndex][j]][1], m->vertices[m->faces[faceIndex][j]][2]);
				eiVector eachNormal = ei_vector(m->normals[m->faces[faceIndex][j]][0], m->normals[m->faces[faceIndex][j]][1], m->normals[m->faces[faceIndex][j]][2]);

				eiVector viewVec = ei_vector(0.0f, 0.0f, 0.0f) - eachVertex;
				eiVector normalProj = eachNormal;
				normalProj = normalize(normalProj) * dot(normalProj, viewVec);
				eiVector w = viewVec - normalProj;

				if (dwk_r[m->faces[faceIndex][j]] / len(w) < t_d) {
					allArePositive = EI_FALSE;
					break;
				}
			}
		}

		if (!allArePositive) {
			faceProcessed[faceIndex] = EI_TRUE;
			continue;
		}

		// HERE WE CHECK THAT theta_c < n(p) dot v(p) / || v(p) ||
		eiBool thetaCIsLess = EI_TRUE;

		for (eiInt j = 0; j < 3; ++j) {
			eiVector eachVertex = ei_vector(m->vertices[m->faces[faceIndex][j]][0], m->vertices[m->faces[faceIndex][j]][1], m->vertices[m->faces[faceIndex][j]][2]);
			eiVector eachNormal = ei_vector(m->normals[m->faces[faceIndex][j]][0], m->normals[m->faces[faceIndex][j]][1], m->normals[m->faces[faceIndex][j]][2]);

			eiVector viewVec = ei_vector(0.0f, 0.0f, 0.0f) - eachVertex;
			eiVector eachNormalTransformed = eachNormal;

			if (!(thetaC < acosf(dot(eachNormalTransformed, viewVec) / len(viewVec)))) {
				thetaCIsLess = EI_FALSE;
				break;
			}
		}

		if (!thetaCIsLess) {
			faceProcessed[faceIndex] = EI_TRUE;
			continue;
		}
	
		// these have to pruned according to the formula D_w*k(r) > 0
		eiVector p1;
		eiVector p2;	
		eiBool drawStroke = EI_FALSE;

		if (dot1 >= 0.0f && dot2 >= 0.0f && dot3 < 0.0f) {				
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = EI_TRUE;
		}
		else if (dot1 < 0.0f && dot2 < 0.0f && dot3 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = EI_TRUE;
		}
		else if (dot2 >= 0.0f && dot3 >= 0.0f && dot1 < 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = EI_TRUE;
		}
		else if (dot2 < 0.0f && dot3 < 0.0f && dot1 >= 0.0f) {
			p1 = interpolateZeroPoint(v1, dot1, v3, dot3);				
			p2 = interpolateZeroPoint(v1, dot1, v2, dot2);
			drawStroke = EI_TRUE;
		}
		else if (dot3 >= 0.0f && dot1 >= 0.0f && dot2 < 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);				
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = EI_TRUE;
		}
		else if (dot3 < 0.0f && dot1 < 0.0f && dot2 >= 0.0f) {
			p1 = interpolateZeroPoint(v2, dot2, v1, dot1);
			p2 = interpolateZeroPoint(v2, dot2, v3, dot3);
			drawStroke = EI_TRUE;
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

		faceProcessed[faceIndex] = EI_TRUE;
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

	ContourChainGroup contourChainGroup;
	contourChainGroup.resetGroup();

	for (size_t i = 0; i < mesh_list.size(); ++i)
	{
		trimesh::TriMesh *m = mesh_list[i];

		std::vector<eiBool> faceVisible;
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
		eiInt chain_color = lfloorf(((eiScalar)chain_index / (eiScalar)num_chains) * 16777215.0f);

		fprintf(file, "<polyline points=\"");

		bool isFirstVector = true;
		for (std::list<eiVector>::iterator vec_iter = chain->contourChain.begin(); 
			vec_iter != chain->contourChain.end(); ++ vec_iter)
		{
			eiVector vec = *vec_iter;

			if (!isFirstVector)
			{
				fprintf(file, ",");
			}

			fprintf(file, "%f %f", fabsf(100.0f * vec.x), fabsf(100.0f * vec.y));

			isFirstVector = false;
		}

		fprintf(file, "\" style=\"fill:white;stroke:#%X;stroke-width:0.25\"/>\n", chain_color);
	}

	fprintf(file, "</svg>\n");
	fclose(file);

	ei_info("Number of contour chains: %d\n", num_chains);

	getch();
	
	return 0;
}
