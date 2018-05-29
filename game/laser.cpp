#include "laser.h"
#include "weapon.h"


void Laser::attachToWeapon(GameObject* w) {
	weapon = w;
	body.parent = &weapon->body;
}

void Laser::buildGeometry() {
	body.loadFromFile("laser.obj");
	updateGeometry();
}

int Laser::updateGeometry() {
	if (abs(position[0]) > 5 && abs(position[1]) > 5) {
		body.scale(glm::vec3(0.0, 0.0, 0.0));
		return -1;
	}
	else {
		body.clearModelMatrix();
		body.position = position;
		body.applyPosition();
		body.scale(size);

		targetSpeed.y = 0.1;
		targetSpeed.z = 0.1;
		update();
		return 1;
	}
}

std::vector<GeometricObject*> Laser::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}
