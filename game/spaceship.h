#pragma once

#include "game_object.h"

#include "weapon.h"

class Spaceship : public GameObject {
	GeometricObject body;
	Weapon weaponLeft;
	Weapon weaponRight;
	
public:

	float barrelRollAngle = 0;

	Spaceship();

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};