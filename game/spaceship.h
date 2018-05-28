#pragma once

#include "game_object.h"

class Spaceship : public GameObject {
	GeometricObject body;
	GameObject weaponLeft;
	GameObject weaponRight;
	
public:

	void buildGeometry();
	void updateGeometry(float angle);
	std::vector<GeometricObject*> getGeometry();
};