#include "scene.h"

void Scene::build() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	GeometricObject s;
	spaceship = s;
	spaceship.loadFromFile("spaceship.obj");
	spaceship.translate(glm::vec3(0.0f, -0.2f, 0.5f));
	spaceship.rotateX(angleX);
	spaceship.pushModelMatrix();
	spaceship.position = glm::vec3(0.0f, -1.0f, 0.0f);
	spaceship.applyPosition();
	
	objects.push_back(&spaceship);

	GeometricObject wl;
	weaponLeft = wl;
	weaponLeft.loadFromFile("SingleWeapon.obj");

	weaponLeft.position = glm::vec3(-0.075f, -0.3f, 0.0f);
	weaponLeft.applyPosition();

	weaponLeft.rotateX(angleX);
	weaponLeft.scale(glm::vec3(0.1f, 0.1f, 0.1f));

	weaponLeft.pushModelMatrix();

	

	objects.push_back(&weaponLeft);

	GeometricObject wr;
	weaponRight = wr;
	weaponRight.loadFromFile("SingleWeapon.obj");

	weaponRight.position = glm::vec3(0.075f, -0.3f, 0.0f);
	weaponRight.applyPosition();

	weaponRight.rotateX(angleX);
	weaponRight.scale(glm::vec3(0.1f, 0.1f, 0.1f));
	weaponRight.pushModelMatrix();
	
	objects.push_back(&weaponRight);

	surface.generate();
	objects.push_back(&surface);
}

void Scene::generateBufferObjects() {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]).generateBufferObjects();
	}
}