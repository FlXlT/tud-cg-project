#include "game_object.h"

void GameObject::move(glm::vec3 movement) {
	position = position + movement;
}

void GameObject::update() {
	move(speed);
}