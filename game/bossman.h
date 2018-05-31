#pragma once

#include "game_object.h"

class bossman : public GameObject {
public:

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();

	void update();
};

