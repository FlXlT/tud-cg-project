#pragma once

#include "game_object.h"

class Laser : public GameObject {

	GameObject* weapon;

public:
	void attachToWeapon(GameObject* weapon);

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};

