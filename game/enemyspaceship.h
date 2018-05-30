#pragma once

#include "game_object.h"

class enemyspaceship : public GameObject {

public:
	enemyspaceship();

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();

	void update();
};

