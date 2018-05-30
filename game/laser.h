#pragma once

#include "game_object.h"

class Laser : public GameObject {

	GameObject* weapon;
	
public:
	void attachToWeapon(GameObject* weapon);

	void buildGeometry();
	int updateGeometry(float x_direction, float y_direction);
	std::vector<GeometricObject*> getGeometry();
};

