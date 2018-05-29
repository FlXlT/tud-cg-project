#pragma once

#include <glm/glm.hpp>

// Per-vertex data
struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};