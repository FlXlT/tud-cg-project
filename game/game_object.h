#pragma once

#include <vector>

#include "geometric_object.h"

class GameObject {

public:

	glm::vec3 position;
	glm::vec3 speed;
	glm::vec3 targetSpeed;

	void move(glm::vec3 movement);
	void update();
};