#include "weapon.h"
#define _USE_MATH_DEFINES
#include "math.h"

void Weapon::attachToSpaceship(GameObject* s) {
	spaceship = s;
	body.parent = &spaceship->body;
}

void Weapon::buildGeometry() {
	body.loadFromFile("SingleWeapon.obj");
	updateGeometry();
}

void Weapon::updateGeometry() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	body.clearModelMatrix();
	body.position = position;
	body.applyPosition();
	body.rotateX(angleY);
	body.scale(size);	
}

std::vector<GeometricObject*> Weapon::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}

float Weapon::computeAngle(float diffX, float diffY) {
	float rotation = 0;
	if (diffY <= 0 && diffX > 0) {
		rotation = -0.5 * M_PI;
	}
	else if (diffY <= 0 && diffX < 0) {
		rotation = 0.5 * M_PI;
	}
	else {
		rotation = -1 * atan(diffX / diffY);
	}
	return rotation;
}