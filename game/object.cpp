// Implements Object
#include "object.h"

void Object::loadFromFile(const char* filename) {
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename)) {
		std::cerr << err << std::endl;
		return;
	}

	// Read triangle vertices from OBJ file
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			// Retrieve coordinates for vertex by index
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			// Retrieve components of normal by index
			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertices.push_back(vertex);
		}
	}
}

std::vector<Vertex>* Object::getVertices() {
	return &vertices;
}

glm::mat4* Object::getModelMatrix() {
	return &model;
}
void Object::setModelMatrix(glm::mat4 matrix) {
	model = matrix;
}
void Object::clearModelMatrix() {
	model = glm::mat4();
}

void Object::translate(glm::vec3 translation) {
	model = glm::translate(model, translation);
}
void Object::translateX(float translation) {
	translate(glm::vec3(translation, 0, 0));
}
void Object::translateY(float translation) {
	translate(glm::vec3(0, translation, 0));
}
void Object::translateZ(float translation) {
	translate(glm::vec3(0, 0, translation));
}

void Object::rotate(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}
void Object::rotateX(float angle) {
	rotate(angle, glm::vec3(1.0, 0, 0));
}
void Object::rotateY(float angle) {
	rotate(angle, glm::vec3(0, 1.0, 0));
}
void Object::rotateZ(float angle) {
	rotate(angle, glm::vec3(0, 0, 1.0));
}

void Object::scale(glm::vec3 factors) {
	model = glm::scale(model, factors);
}
void Object::scaleX(float factor) {
	scale(glm::vec3(factor, 0, 0));
}
void Object::scaleY(float factor) {
	scale(glm::vec3(0, factor, 0));
}
void Object::scaleZ(float factor) {
	scale(glm::vec3(0, 0, factor));
}
