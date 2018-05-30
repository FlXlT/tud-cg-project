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
	void Spaceship::shootLaser(float mouseX, float mouseY);

	bool collidesLeft();
	bool collidesRight();
	bool collidesUp();
	bool collidesDown();

	void update();
};