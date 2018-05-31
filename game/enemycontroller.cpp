#include "enemycontroller.h"

enemycontroller::enemycontroller()
{
	/*enemies[0] = enemy1;
	enemies[1] = enemy2;
	enemies[2] = enemy3;
	enemies[3] = enemy4;
	enemies[4] = enemy5;*/
}

void enemycontroller::init()
{
	for (int i = 0; i < amountEnemies; i++)
	{
		enemies[i].buildGeometry();
	}
}

void enemycontroller::update() {

}


