#pragma once

#include <vector>

#include "geometric_object.h"
#include "spaceship.h"
#include "terrain.h"

class Scene {
	
	
public:

	std::vector<GameObject*> objects;

	std::vector<GeometricObject*> getGeometricObjects();

	Spaceship spaceship;
	Terrain terrain;

	GeometricObject cursor;

	void build();
	void generateBufferObjects();

	void handleKey(int key, int action);

	void update();
};