#include "laser.h"
#include "weapon.h"


void Laser::attachToWeapon(GameObject* w) {
	weapon = w;
	body.parent = &weapon->body;
}

void Laser::buildGeometry() {
	body.loadFromFile("laser.obj");
	updateGeometry(0.0,0.0);
}

int Laser::updateGeometry(float x_direction, float y_direction) {
	if (abs(position[0]) > 5 && abs(position[1]) > 5) {
		targetSpeed.x = 0.0;  // horizontal
		targetSpeed.z = 0.0;  // vertical 
		body.scale(glm::vec3(0.0, 0.0, 0.0));
		return -1;
	}
	else {
		body.clearModelMatrix();
		body.position = position;
		body.applyPosition();
		body.scale(size);

		std::cout << x_direction << std::endl;
		std::cout << y_direction << std::endl;
		std::cout << "haha2" << std::endl;

		targetSpeed.x = x_direction;  // horizontal
		targetSpeed.z = y_direction;  // vertical 
		update();
		return 1;
	}
}

std::vector<GeometricObject*> Laser::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}
