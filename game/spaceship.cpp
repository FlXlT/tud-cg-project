#include "spaceship.h"

Spaceship::Spaceship() {
	position = glm::vec3(0.0f, -0.2f, 2.0f);
}

void Spaceship::buildGeometry() {
	body.loadFromFile("spaceship.obj");

	weaponLeft.attachToSpaceship(this);
	weaponLeft.position = glm::vec3(-0.25f, 0.875f, -0.125f);
	weaponLeft.buildGeometry();

	weaponRight.attachToSpaceship(this);
	weaponRight.position = glm::vec3(0.25f, 0.875f, -0.125f);
	weaponRight.buildGeometry();

	updateGeometry();
}

void Spaceship::updateGeometry() {
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

	weaponLeft.updateGeometry();
	weaponRight.updateGeometry();
}

std::vector<GeometricObject*> Spaceship::getGeometry() {
	std::vector<GeometricObject*> geometry;
	std::vector<GeometricObject*> g;

	// Add own body
	geometry.push_back(&body);
	// Add geometry of left weapon
	g = weaponLeft.getGeometry();
	geometry.insert(geometry.end(), g.begin(), g.end());
	// Add geometry of right weapon
	g = weaponRight.getGeometry();
	geometry.insert(geometry.end(), g.begin(), g.end());

	return geometry;
}

void Spaceship::shootLaser() {
	weaponLeft.shootLaser();
	weaponRight.shootLaser();
}

bool Spaceship::collidesLeft() {
	if (position.x <= -2.5f) {
		return true;
	}
	return false;
}

bool Spaceship::collidesRight() {
	if (position.x >= 2.5f) {
		return true;
	}
	return false;
}

bool Spaceship::collidesUp() {
	if (position.y >= 1.65f) {
		return true;
	}
	return false;
}

bool Spaceship::collidesDown() {
	if (position.y <= -2.0f) {
		return true;
	}
	return false;
}

// Overrides GameObject::update() because of spaceship specific check.
void Spaceship::update() {
	if (targetSpeed.x < 0.0 && collidesLeft() || targetSpeed.x > 0.0 && collidesRight()) {
		speed.x = 0;
	} else {
		speed.x += (targetSpeed.x - speed.x) / 10;
	}

	if (targetSpeed.y < 0.0 && collidesDown() || targetSpeed.y > 0.0 && collidesUp()) {
		speed.y = 0;
	}
	else {
		speed.y += (targetSpeed.y - speed.y) / 10;
	}

	speed.z += (targetSpeed.z - speed.z) / 10;

	move(speed);
}

