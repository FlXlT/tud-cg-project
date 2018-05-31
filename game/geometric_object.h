#pragma once

#include <GL/glew.h>

// Vector datastructure
#include <vector>
// String datastructure
#include <string>
// Vertex datastructure
#include "vertex.h"

// For writing errors to standard output stream
#include <iostream>

// Library for loading .OBJ model
#ifndef TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#endif

// Library for vertex and matrix math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// random number generation
#include <cstdlib>

// PI constant
#define _USE_MATH_DEFINES
#include <math.h>

namespace surface_modes {
	const unsigned int random = 0;
	const unsigned int sinusoidal = 1;
}

class GeometricObject {
	// List of vertices that constitute the geometric representation of this object
	std::vector<Vertex> vertices;

	// The model matrix for this object
	glm::mat4 model;
	glm::mat4 completeModel;
	std::vector<glm::mat4> modelStack;

	// Tiny OBJ Loader Related Attributes
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

public:

	// Parent geometric object to which this object is "attached"
	GeometricObject * parent;

	glm::vec3 position;

	glm::vec3 diffuseColor = glm::vec3(1.0f);
	glm::vec3 specularColor = glm::vec3(1.0f);
	float specularIntensity = 64;
	bool useTex = false;

	GLuint vbo;
	GLuint vao;

	GeometricObject();
	GeometricObject(GeometricObject* parent);

	void generateBufferObjects();

	void loadFromFile(const char* filename);
	void generateSurface(const unsigned int mode, const float width, const float height, const float depth, const int nbQuadsX, const int nbQuadsY, const int nbQuadsPerTexX, const int nbQuadsPerTexY, const int periodsX, const int periodsY);
	std::vector<Vertex>* getVertices();
	int size();
	Vertex* data();

	glm::mat4* getModelMatrix();
	void setModelMatrix(glm::mat4 matrix);
	void clearModelMatrix();
	void pushModelMatrix();
	void popModelMatrix();
	void loadModelMatrix();
	void applyPosition();

	void translate(glm::vec3 translation);
	void translateX(float translation);
	void translateY(float translation);
	void translateZ(float translation);

	void rotate(float angle, glm::vec3 axis);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void scale(glm::vec3 factors);
	void scaleX(float factor);
	void scaleY(float factor);
	void scaleZ(float factor);
};
