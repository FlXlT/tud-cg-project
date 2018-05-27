#pragma once

#include <vector>

#include "geometric_object.h"

class Scene {
	std::vector<GeometricObject> objects;

public:

	void build();
	std::vector<GeometricObject>* getObjects();
};