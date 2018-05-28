#include "scene.h"

// for event handling
#include <GLFW/glfw3.h>

void Scene::build() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	GeometricObject* spaceshipGeom;
	spaceship.buildGeometry();
	std::vector<GeometricObject*> geometry = spaceship.getGeometry();
	for (int i = 0; i < geometry.size(); i++) {
		spaceshipGeom = geometry[i];
		objects.push_back(spaceshipGeom);
	}


	weaponLeft.loadFromFile("SingleWeapon.obj");
	weaponLeft.parent = spaceshipGeom;

	//weaponLeft.position = glm::vec3(-0.5f, -0.3f, 0.5f);
	//weaponLeft.applyPosition();
	weaponLeft.translate(glm::vec3(-0.25f, 0.875f, -0.075f));
	weaponLeft.rotateX(angleY);
	weaponLeft.scale(glm::vec3(1.0f, 1.0f, 1.0f));
	
	
	objects.push_back(&weaponLeft);


	weaponRight.loadFromFile("SingleWeapon.obj");
	//weaponRight.parent = spaceshipGeom;

	//weaponRight.position = glm::vec3(0.5f, -0.3f, 0.5f);
	//weaponRight.applyPosition();
	//weaponRight.rotateX(angleX);
	weaponRight.scale(glm::vec3(3.0f, 3.0f, 3.0f));
	//objects.push_back(&weaponRight);

	surface.generate();
	objects.push_back(&surface);
}

void Scene::generateBufferObjects() {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]).generateBufferObjects();
	}
}

void Scene::handleKey(int key, int action) {
	switch (key) {
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS)   spaceship.speed.x = -0.05;
		if (action == GLFW_RELEASE) spaceship.speed.x = 0.0;
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS)   spaceship.speed.x = 0.05;
		if (action == GLFW_RELEASE) spaceship.speed.x = 0.0;
		break;
	}
}

void Scene::update() {
	spaceship.update();
	spaceship.updateGeometry();
}