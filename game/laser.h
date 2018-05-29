#pragma once

#include "game_object.h"

class Laser : public GameObject {

	GameObject* weapon;
	
public:
	void attachToWeapon(GameObject* weapon);

	void buildGeometry();
	int updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};

