#include "laser.h"
#include "weapon.h"


void Laser::attachToWeapon(GameObject* w) {
	weapon = w;
	body.parent = &weapon->body;
}

void Laser::buildGeometry() {
	body.loadFromFile("EnemySpaceShip.obj");
	updateGeometry();
}

int Laser::updateGeometry() {
	if (abs(position[0]) > 10 && abs(position[1]) > 10) {
		return -1;
	}
	else {
		body.clearModelMatrix();
		body.position = position;
		body.applyPosition();
		std::cout << position[0] << std::endl;
		//body.rotateX();
		//body.scale();
		return 1;
	}
}

std::vector<GeometricObject*> Laser::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}
