#include "weapon.h"

void Weapon::buildGeometry() {
	body.loadFromFile("SingleWeapon.obj");
	body.position = glm::vec3(0.0f, -1.0f, 0.0f);
	updateGeometry();
}

void Weapon::updateGeometry() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	body.clearModelMatrix();
	body.translate(glm::vec3(0.0f, -0.2f, 0.5f));
	body.rotateX(angleX);
	body.applyPosition();
}

std::vector<GeometricObject*> Weapon::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}