#include "game_object.h"

void GameObject::move(glm::vec3 movement) {
	position = position + movement;
}

void GameObject::update() {
	speed.x += (targetSpeed.x - speed.x) / 10;
	speed.y += (targetSpeed.y - speed.y) / 10;
	speed.z += (targetSpeed.z - speed.z) / 10;

	move(speed);
}