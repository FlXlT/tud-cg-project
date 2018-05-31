#pragma once

#ifdef WIN32
#include <windows.h>
#include <vector>
#endif
#include "game_object.h"
#include "mesh.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "grid.h"
#include "Vertex1.h"

class bossman : public GameObject {
public:

	void buildGeometry();
	void updateGeometry();
	GeometricObject MeshToGeometricObject(Mesh meshToGeo);
	Mesh simplifyMesh(unsigned int r, Mesh mesh);
	std::vector<GeometricObject*> getGeometry();
	GeometricObject convertedSimpleMesh128;
	GeometricObject convertedSimpleMesh64;
	GeometricObject convertedSimpleMesh32;
	GeometricObject convertedSimpleMesh16;
	void update();
};

