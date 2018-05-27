#include "scene.h"

void Scene::build() {
	GeometricObject spaceship;
	spaceship.loadFromFile("spaceship.obj");

	objects.push_back(spaceship);
}