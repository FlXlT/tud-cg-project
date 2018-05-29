#pragma once

#include "game_object.h"
#include "laser.h"

class Weapon : public GameObject {

	GameObject* spaceship;
	Laser laserBeam;
	int laserOOB;

public:

	void attachToSpaceship(GameObject* spaceship);

	void Weapon::shootLaser();
	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
	void Weapon::detachLaser();
};