#pragma once

#include "game_object.h"

class Weapon : public GameObject {

	GameObject* spaceship;

public:

	void attachToSpaceship(GameObject* spaceship);

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();

	static float computeAngle(float diffY, float diffX);
};