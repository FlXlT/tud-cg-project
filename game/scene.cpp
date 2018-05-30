#include "scene.h"

// for event handling
#include <GLFW/glfw3.h>

void Scene::build() {
	spaceship.buildGeometry();
	objects.push_back(&spaceship);

	terrain.buildGeometry();
	objects.push_back(&terrain);

	cursor.diffuseColor = glm::vec3(1.0f, 0, 0);
	cursor.loadFromFile("assets/models/cursor.obj");
}

std::vector<GeometricObject*> Scene::getGeometricObjects() {
	std::vector<GeometricObject*> geometricObjects;

	for (int o = 0; o < objects.size(); o++) {
		std::vector<GeometricObject*> geometry = objects[o]->getGeometry();
		for (int i = 0; i < geometry.size(); i++) {
			geometricObjects.push_back(geometry[i]);
		}
	}

	geometricObjects.push_back(&cursor);

	return geometricObjects;
}

void Scene::generateBufferObjects() {
	std::vector<GeometricObject*> geometricObjects = getGeometricObjects();
	for (int i = 0; i < geometricObjects.size(); i++) {
		(*geometricObjects[i]).generateBufferObjects();
	}
}

void Scene::handleKey(int key, int action) {
	switch (key) {
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)   spaceship.targetSpeed.x = -0.05;
		if (action == GLFW_RELEASE) spaceship.targetSpeed.x = 0.0;
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)   spaceship.targetSpeed.x = 0.05;
		if (action == GLFW_RELEASE) spaceship.targetSpeed.x = 0.0;
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)   spaceship.targetSpeed.y = 0.05;
		if (action == GLFW_RELEASE) spaceship.targetSpeed.y = 0.0;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)   spaceship.targetSpeed.y = -0.05;
		if (action == GLFW_RELEASE) spaceship.targetSpeed.y = 0.0;
		break;
	}
}

void Scene::update() {
	spaceship.update();
	spaceship.updateGeometry();

	terrain.update();
	terrain.updateGeometry();

	cursor.clearModelMatrix();
	//cursor.position.z = spaceship.position.z;
	cursor.applyPosition();
	cursor.scale(glm::vec3(0.25f, 0.25f, 0.25f));
}
