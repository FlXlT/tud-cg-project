#include "spaceship.h"

void Spaceship::buildGeometry() {
	body.loadFromFile("spaceship.obj");
	body.position = glm::vec3(0.0f, -0.2f, 0.5f);
	updateGeometry(0.0f);
}

void Spaceship::updateGeometry(float angle) {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	body.clearModelMatrix();
	body.applyPosition();
	body.rotateY(angle);
	body.rotateX(angleX);
	body.translate(glm::vec3(0.0f, -1.0f, 0.0f));
}

std::vector<GeometricObject*> Spaceship::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}