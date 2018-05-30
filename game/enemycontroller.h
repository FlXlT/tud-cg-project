#pragma once

#include <vector>
#include "geometric_object.h"
#include "game_object.h"
#include "enemyspaceship.h"

class enemycontroller
{
public:
	enemycontroller();

	std::vector<GameObject*> spawnlocations;
	std::vector<enemyspaceship> enemies = {enemy1, enemy2, enemy3, enemy4, enemy5};
	int amountEnemies = 3;

	enemyspaceship enemy1;
	enemyspaceship enemy2;
	enemyspaceship enemy3;
	enemyspaceship enemy4;
	enemyspaceship enemy5;

	void init();
	void update();
};

