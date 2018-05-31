#pragma once

#include "game_object.h"

class bossman : public GameObject {
public:

	void buildGeometry();
	void updateGeometry();
	GeometricObject MeshToGeometricObject();
	std::vector<GeometricObject*> getGeometry();
	GeometricObject convertedSimpleMesh128;
	GeometricObject convertedSimpleMesh64;
	GeometricObject convertedSimpleMesh32;
	GeometricObject convertedSimpleMesh16;
	void update();
};

