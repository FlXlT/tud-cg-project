#pragma once

#include "game_object.h"

class Weapon : public GameObject {
	GeometricObject body;

public:

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};