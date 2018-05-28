#include "spaceship.h"

Spaceship::Spaceship() {
	position = glm::vec3(0.0f, -0.2f, 1.0f);
}

void Spaceship::buildGeometry() {
	body.loadFromFile("spaceship.obj");
	updateGeometry();
}

void Spaceship::updateGeometry() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	barrelRollAngle = speed.x * 10;
	float tiltAngle = speed.y * 10;

	body.clearModelMatrix();
	body.position = position;
	body.applyPosition();
	body.rotateY(barrelRollAngle);
	body.rotateX(angleX - tiltAngle);
	body.translate(glm::vec3(0.0f, -1.0f, 0.0f));
}

std::vector<GeometricObject*> Spaceship::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}