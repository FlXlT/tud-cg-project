#include "scene.h"

void Scene::build() {
	float angleX = -90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	GeometricObject s;
	spaceship = s;
	spaceship.loadFromFile("spaceship.obj");
	spaceship.translate(glm::vec3(0.0f, -0.5f, 0.0f));
	spaceship.scale(glm::vec3(0.5f, 0.5f, 0.5f));
	spaceship.rotateX(angleX);
	spaceship.rotateY(angleY);
	spaceship.pushModelMatrix();
	objects.push_back(&spaceship);

	GeometricObject wl;
	weaponLeft = wl;
	weaponLeft.loadFromFile("SingleWeapon.obj");
	weaponLeft.translate(glm::vec3(-0.125f, -0.42f, 0.0f));
	weaponLeft.scale(glm::vec3(0.05f, 0.05f, 0.05f));
	weaponLeft.rotateX(angleX);
	weaponLeft.pushModelMatrix();
	objects.push_back(&weaponLeft);

	GeometricObject wr;
	weaponRight = wr;
	weaponRight.loadFromFile("SingleWeapon.obj");
	weaponRight.translate(glm::vec3(0.125f, -0.42f, 0.0f));
	weaponRight.scale(glm::vec3(0.05f, 0.05f, 0.05f));
	weaponRight.rotateX(angleX);
	weaponRight.pushModelMatrix();
	objects.push_back(&weaponRight);
}

void Scene::generateBufferObjects() {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]).generateBufferObjects();
	}
}