#pragma once

#include <vector>
#include <ctime>

#include "geometric_object.h"
#include "spaceship.h"
#include "laser.h"

class Scene {
	bool      leftMousePressed = false;
	std::time_t lastTimeShot = std::time(0);
	
public:

	std::vector<GeometricObject*> objects;

	Spaceship spaceship;
	GeometricObject laser1;
	GeometricObject laser2;
	GeometricObject weaponLeft;
	GeometricObject weaponRight;
	GeometricObject surface;

	void build();
	void generateBufferObjects();

	void handleKey(int key, int action);
	void sceneMouseButtonHandler(int button, int action, float mouseXcoord, float mouseYcoord);

	void update();
};