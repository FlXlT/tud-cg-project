#pragma once

#include <vector>

#include "geometric_object.h"

class Scene {
	
	
public:

	std::vector<GeometricObject*> objects;

	GeometricObject spaceship;
	GeometricObject weaponLeft;
	GeometricObject weaponRight;
	GeometricObject surface;

	void build();
	void generateBufferObjects();
};