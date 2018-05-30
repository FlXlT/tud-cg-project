#pragma once

#include "game_object.h"

class Weapon : public GameObject {

	GameObject* spaceship;
	

public:

	float angle;

	void attachToSpaceship(GameObject* spaceship);

	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();

	static float computeAngle(float diffY, float diffX);

	void updateAngle(GeometricObject obj);
	void updateAngle(glm::mat4 mvp, GeometricObject obj, float mouseXcoord, float mouseYcoord);
};