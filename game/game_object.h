#pragma once

#include <vector>

#include "geometric_object.h"

class GameObject {

public:

	GeometricObject body;

	glm::vec3 position;
	glm::vec3 speed;
	glm::vec3 targetSpeed;
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);

	void move(glm::vec3 movement);
	void update();
};