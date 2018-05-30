#include "weapon.h"
#define _USE_MATH_DEFINES
#include "math.h"

void Weapon::attachToSpaceship(GameObject* s) {
	spaceship = s;
	body.parent = &spaceship->body;
	angle = 0;
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

void Weapon::updateAngle(GeometricObject obj) {
	obj.rotateY(angle);
}

void Weapon::updateAngle(glm::mat4 mvp, GeometricObject obj, float mouseXcoord, float mouseYcoord) {
	glm::vec4 screenposSpaceship = mvp * glm::vec4(spaceship->position, 1.0);
	
	glm::vec4 screenposWeapon = mvp * glm::vec4(obj.position, 1.0);
	screenposWeapon = screenposWeapon + screenposSpaceship;

	float diffX = screenposWeapon.x - mouseXcoord;
	float diffY = -(screenposWeapon.y - 1.629f) - mouseYcoord;

	// Clamp weapons if aiming down
	float angleWeapon = computeAngle(diffX, diffY);
	angle = angleWeapon;
}