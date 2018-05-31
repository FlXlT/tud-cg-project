#include "bossman.h"
	
void bossman::buildGeometry()
{
	body.diffuseColor = glm::vec3(0.75f, 0.75f, 0.75f);
	//body.loadFromFile("spaceship.obj");   // LOAD FROM MESH :)
	updateGeometry();
}

void bossman::updateGeometry() {

}

std::vector<GeometricObject*> bossman::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);

	return geometry;
}

void bossman::update() {

}

