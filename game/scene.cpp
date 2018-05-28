#include "scene.h"

void Scene::build() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	GeometricObject s;
	spaceship = s;
	spaceship.loadFromFile("spaceship.obj");
	spaceship.rotateX(angleX);
	spaceship.pushModelMatrix();
	
	//spaceship.scale(glm::vec3(0.5f, 0.5f, 0.5f));
	//spaceship.rotateX(angleX);
	//spaceship.rotateY(angleY);
	spaceship.position = glm::vec3(0.0f, -1.0f, 0.0f);
	spaceship.applyPosition();
	
	

	
	
	
	objects.push_back(&spaceship);

	GeometricObject wl;
	weaponLeft = wl;
	weaponLeft.loadFromFile("SingleWeapon.obj");
	weaponLeft.scale(glm::vec3(0.05f, 0.05f, 0.05f));
	weaponLeft.rotateX(angleX);
	
	weaponLeft.pushModelMatrix();

	weaponLeft.position = glm::vec3(-0.075f, -0.3f, 0.0f);
	weaponLeft.applyPosition();

	objects.push_back(&weaponLeft);

	GeometricObject wr;
	weaponRight = wr;
	weaponRight.loadFromFile("SingleWeapon.obj");
	weaponRight.scale(glm::vec3(0.05f, 0.05f, 0.05f));
	weaponRight.rotateX(angleX);
	
	weaponRight.pushModelMatrix();

	weaponRight.position = glm::vec3(0.075f, -0.3f, 0.0f);
	weaponRight.applyPosition();
	
	objects.push_back(&weaponRight);
}

void Scene::generateBufferObjects() {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]).generateBufferObjects();
	}
}