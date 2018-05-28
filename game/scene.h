#pragma once

#include <vector>

#include "geometric_object.h"
#include "spaceship.h"

class Scene {
	
	
public:

	std::vector<GeometricObject*> objects;

	Spaceship spaceship;
	GeometricObject weaponLeft;
	GeometricObject weaponRight;
	GeometricObject surface;

	void build();
	void generateBufferObjects();
};