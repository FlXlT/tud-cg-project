#include "laser.h"
#include "weapon.h"


void Laser::attachToWeapon(GameObject* w) {
	weapon = w;
	body.parent = &weapon->body;
}

void Laser::buildGeometry() {
	body.loadFromFile("laser.obj");
	body.diffuseColor = glm::vec3(1.0f, 0, 0);
	body.scale(glm::vec3(0.25f, 0.25f, 0.25f));
	targetSpeed.x = 0.0;
	targetSpeed.y = 0.0;
	speed.x = 0.0;
	speed.y = 0.0;
	updateGeometry();
}

void Laser::updateGeometry(){
	if (abs(position[0]) > 5 && abs(position[1]) > 5) {
		targetSpeed.x = 0.0;  // horizontal
		targetSpeed.z = 0.0;  // vertical 
		speed.x = 0.0;
		speed.y = 0.0;
		body.scale(glm::vec3(0.0, 0.0, 0.0));
	}
	else {
		body.clearModelMatrix();
		body.position = position;
		body.applyPosition();
		body.diffuseColor = glm::vec3(1.0f, 0, 0);
		body.scale(glm::vec3(0.25f, 0.25f, 0.25f));
		update();
	}
}

std::vector<GeometricObject*> Laser::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}
