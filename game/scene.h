#pragma once

#include <vector>
#include <ctime>

#include "geometric_object.h"
#include "spaceship.h"
#include "laser.h"
#include "terrain.h"
#include "enemycontroller.h"
#include "bossman.h"

class Scene {
	bool		leftMousePressed = false;
	bool		before = false;
	

	std::time_t lastTimeShot = std::time(0);
	
public:
	int	hitcount = 0;

	std::vector<GameObject*> objects;

	std::vector<GeometricObject*> getGeometricObjects();

	Spaceship spaceship;
	bossman bowsman;
	GeometricObject laser1;
	GeometricObject laser2;
	GeometricObject weaponLeft;
	GeometricObject weaponRight;
	GeometricObject surface;
	Terrain terrain;
	enemycontroller enemyController;

	glm::vec2 mouseProjection;
	glm::vec2 spaceshipProjection;

	GeometricObject cursor;

	void build();
	void generateBufferObjects();
	void generateLaserBufferObjects();
	void generateEnemyBufferObjects();

	void handleKey(int key, int action);
	void sceneMouseButtonHandler(int button, int action, float mouseXcoord, float mouseYcoord);

	void update();
};