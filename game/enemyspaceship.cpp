#include "enemyspaceship.h"

enemyspaceship::enemyspaceship()
{
	position = glm::vec3(5.0, 0.0f, 5.0f);
}

void enemyspaceship::buildGeometry() {
	body.loadFromFile("enemyspaceship.obj");
	updateGeometry();
}

std::vector<GeometricObject*> enemyspaceship::getGeometry() {
	std::vector<GeometricObject*> geometry;
	return geometry;
}

void enemyspaceship::updateGeometry() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	float barrelRollAngle = speed.x * 10;
	float tiltAngle = speed.y * 10;

	body.clearModelMatrix();
	body.position = position;
	body.applyPosition();
	body.rotateY(barrelRollAngle);
	body.rotateX(angleX - tiltAngle);
	body.scale(size);
	body.translate(glm::vec3(0.0f, -1.0f, 0.0f));
}

void enemyspaceship::update() {
	if (targetSpeed.x < 0.0 || targetSpeed.x > 0.0) {
		speed.x = 0;
	}
	else {
		speed.x += (targetSpeed.x - speed.x) / 10;
	}

	if (targetSpeed.y < 0.0|| targetSpeed.y > 0.0) {
		speed.y = 0;
	}
	else {
		speed.y += (targetSpeed.y - speed.y) / 10;
	}

	speed.z += (targetSpeed.z - speed.z) / 10;

	move(speed);
}