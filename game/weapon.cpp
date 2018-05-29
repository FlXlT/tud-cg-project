#include "weapon.h"
#define _USE_MATH_DEFINES
#include "math.h"

void Weapon::attachToSpaceship(GameObject* s) {
	spaceship = s;
	body.parent = &spaceship->body;
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

	laserOOB = laserBeam.updateGeometry();
	//if (laserOOB == -1) {
	//	//laserBeam = NULL;
	//}
}

void Weapon::shootLaser() {
	laserBeam.attachToWeapon(this);
	laserBeam.position = glm::vec3(0.0f, 0.0f, 0.0f);
	laserBeam.buildGeometry();

	updateGeometry();

	laserBeam.targetSpeed.x = 0.05;
	laserBeam.targetSpeed.y = 0.05;
	laserBeam.update();
}

std::vector<GeometricObject*> Weapon::getGeometry() {
	std::vector<GeometricObject*> geometry;
	std::vector<GeometricObject*> g;
	
	// Add own body
	geometry.push_back(&body);

	// Add geometry of bullet
	g = laserBeam.getGeometry();
	geometry.insert(geometry.end(), g.begin(), g.end());

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