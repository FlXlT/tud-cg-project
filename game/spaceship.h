#pragma once

#include "game_object.h"

#include "weapon.h"

class Spaceship : public GameObject {

public:

	Weapon weaponLeft;
	Weapon weaponRight;

	Spaceship();

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};