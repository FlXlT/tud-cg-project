#pragma once

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

class Object {
	// List of vertices that constitute the geometric representation of this object
	std::vector<Vertex> vertices;

	// Tiny OBJ Loader Related Attributes
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

public:

	void loadFromFile(const char* filename);
	std::vector<Vertex>* getVertices();
};