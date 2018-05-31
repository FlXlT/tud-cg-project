#pragma once

#include "game_object.h"
#include "laser.h"

class Weapon : public GameObject {

	GameObject* spaceship;
	Laser laserBeam;
	int laserOOB;

public:

	float angle;

	void attachToSpaceship(GameObject* spaceship);

	void Weapon::shootLaser(float x_direction, float y_direction);
	void buildGeometry();
	void updateGeometry();
	std::vector<GeometricObject*> getGeometry();

	static float computeAngle(float diffY, float diffX);

	void updateAngle(GeometricObject obj);
	void updateAngle(glm::mat4 mvp, GeometricObject obj, float mouseXcoord, float mouseYcoord);
};