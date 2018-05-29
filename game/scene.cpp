#include "scene.h"

// for event handling
#include <GLFW/glfw3.h>

void Scene::build() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	spaceship.buildGeometry();
	std::vector<GeometricObject*> geometry = spaceship.getGeometry();
	for (int i = 0; i < geometry.size(); i++) {
		objects.push_back(geometry[i]);
	}

	surface.diffuseColor = { 133.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f };
	surface.generate();
	surface.specularColor = { 0, 0, 0 };
	surface.specularIntensity = 0;
	surface.useTex = true;
	objects.push_back(&surface);
}

void Scene::generateBufferObjects() {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]).generateBufferObjects();
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
}

