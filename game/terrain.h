#pragma once

#include "game_object.h"

class Terrain : public GameObject {

public:
	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};