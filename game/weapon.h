#pragma once

#include "game_object.h"

class Weapon : public GameObject {
	GeometricObject body;

	GameObject* parent;

public:

	void setParent(GameObject* parent);

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();
};