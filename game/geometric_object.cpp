// Implements Object
#include "geometric_object.h"

GeometricObject::GeometricObject() {
	parent = NULL;
	model = glm::mat4();
}
GeometricObject::GeometricObject(GeometricObject* p) {
	parent = p;
	model = glm::mat4();
}

// terrain generation, had a little bit too much fun with this, hence all the parameters
void GeometricObject::generateSurface(const unsigned int mode, const float width, const float height, const float depth, const int nbQuadsX, const int nbQuadsY, const int nbQuadsPerTexX, const int nbQuadsPerTexY, const int periodsX, const int periodsY) {
	const int nbQuads = nbQuadsX * nbQuadsY;

	const int nbVertsX = nbQuadsX + 1;
	const int nbVertsY = nbQuadsY + 1;
	const int nbVerts = nbVertsX * nbVertsY;

	const float quadWidth = width / nbQuadsX;
	const float quadHeight = height / nbQuadsY;

	glm::vec2 texSizePerQuad = {
		1.0f / (float)nbQuadsPerTexX,
		1.0f / (float)nbQuadsPerTexY
	};

	// center of generated geometry should be at position
	glm::vec3 start = {
		position.x - width / 2,
		position.y - height / 2,
		position.z
	};

	// generate random z coordinates
	float minZ = 0;
	float maxZ = depth;
	
	std::vector<float> zCoords;
	for (int zi = 0; zi < nbVerts; zi++) {
		float z = minZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxZ - minZ)));
		zCoords.push_back(z);
	}

	std::vector<glm::vec3> positions;
	positions.resize(nbVerts);
	std::vector<glm::vec3> normals;
	normals.resize(nbVerts);

	// generate positions
	for (int vx = 0; vx < nbVertsX; vx++) {
		for (int vy = 0; vy < nbVertsY; vy++) {
			int v = vx + vy * (nbVertsX);

			glm::vec3 offset = {
				vx * quadWidth,
				vy * quadHeight,
				0
			};

			glm::vec3 pos;

			// the surface mode determines the distribution of z coordinates
			if (mode == surface_modes::random) {
				// assign consistent random z coordinate
				pos.z = zCoords[v];
			}
			else if (mode == surface_modes::sinusoidal) {
				const float progressionX = periodsX * (float)vx / (float)nbVertsX;
				const float progressionY = periodsY * (float)vy / (float)nbVertsY;

				const float angleX = 2 * M_PI * progressionX;
				const float angleY = 2 * M_PI * progressionY;

				pos.z = depth * (0.5f * sin(angleX) + 0.5f * sin(angleY));

				const float normalAngleY = (0.3f * M_PI * cos(angleX));
				const float normalAngleX = (0.3f * M_PI * cos(angleY));
				glm::mat3 rotationY = glm::mat3(glm::rotate(glm::mat4(), normalAngleY, glm::vec3(0, 1, 0)));
				glm::mat3 rotationX = glm::mat3(glm::rotate(glm::mat4(), normalAngleX, glm::vec3(1, 0, 0)));
				normals[v] = rotationY * rotationX * glm::vec3(0, 0, 1);
			}
			

			// make coordinates absolute by application of start position and offset
			pos = pos + start + offset;

			// index position
			positions[v] = pos;
		}
	}


	// generate normals
	if (mode == surface_modes::random) {
		int dx = 1;
		int dy = nbVertsX;
		int minX = 0;
		int maxX = nbVertsX - 1;
		int minY = 0;
		int maxY = nbVertsY - 1;

		for (int vx = 0; vx < nbVertsX; vx++) {
			for (int vy = 0; vy < nbVertsY; vy++) {
				int v = vx + vy * (nbVertsX);

				glm::vec3 a = positions[v];

				bool left = vx > minX;
				bool right = vx < maxX;
				bool top = vy < maxY;
				bool bottom = vy > minY;

				// Face normal interpolation

				glm::vec3 normalFaces = glm::vec3(0.0f, 0.0f, 0.0f);

				std::vector<glm::vec2> triangleIndices;

				if (left && top) {
					triangleIndices.push_back(glm::vec2(v + dy, v + dy - dx));
				}
				if (right && top) {
					triangleIndices.push_back(glm::vec2(v + dx + dy, v + dy));
					triangleIndices.push_back(glm::vec2(v + dx, v + dx + dy));
				}
				if (right && bottom) {
					triangleIndices.push_back(glm::vec2(v - dy, v + dx));
				}
				if (left && bottom) {
					triangleIndices.push_back(glm::vec2(v - dx - dy, v - dy));
					triangleIndices.push_back(glm::vec2(v - dx, v - dx - dy));
				}

				for (int i = 0; i < triangleIndices.size(); i++) {
					glm::vec3 b = positions[triangleIndices[i][0]];
					glm::vec3 c = positions[triangleIndices[i][1]];
					normalFaces += dot(b - a, c - a);
				}
				normalize(normalFaces);

				// Centroid normal interpolation

				glm::vec3 normalCentroid = glm::vec3(0.0f, 0.0f, 0.0f);

				std::vector<int> neighbourIndices;

				if (vx > minX) {
					neighbourIndices.push_back(v - dx);
				}
				if (vx < maxX) {
					neighbourIndices.push_back(v + dx);
				}
				if (vy > minY) {
					neighbourIndices.push_back(v - dy);
				}
				if (vy < maxY) {
					neighbourIndices.push_back(v + dy);
				}
				if (vx > minX && vy > minY) {
					neighbourIndices.push_back(v - dx - dy);
				}
				if (vx < maxX && vy < maxY) {
					neighbourIndices.push_back(v + dx + dy);
				}
				if (vx > minX && vy < maxY) {
					//neighbourIndices.push_back(v - dx + dy);
				}
				if (vx < maxX && vy > minY) {
					//neighbourIndices.push_back(v + dx - dy);
				}

				for (int i = 0; i < neighbourIndices.size(); i++) {
					normalCentroid += a - positions[neighbourIndices[i]];
				}
				normalize(normalCentroid);
				// dirty trick to make sure all normals are on the right side of the surface
				if (normalCentroid.z < 0) {
					normalCentroid = glm::vec3(-1) * normalCentroid;
				}

				glm::vec3 normal = normalFaces + normalCentroid;
				normalize(normal);

				normals[v] = normalCentroid;
			}
		}
	}


	// generate vertices
	for (int qx = 0; qx < nbQuadsX; qx++) {
		for (int qy = 0; qy < nbQuadsY; qy++) {
			glm::vec2 texCoordsOffset = {
				(float)(qx % nbQuadsPerTexX),
				nbQuadsPerTexY - 1 - (float)(qy % nbQuadsPerTexY)
			};
			texCoordsOffset = texCoordsOffset * texSizePerQuad;

			// Generate quad
			for (int i = 0; i < 6; i++) {
				int vx;
				int vy;
				Vertex vertex = {};

				// compute relative coordinates of quad vertex
				if (i == 0 || i == 3) {
					vertex.texCoords = { 0.0f, 1.0f };
					vx = qx;
					vy = qy;
				}
				if (i == 1) {
					vertex.texCoords = { 1.0f, 1.0f };
					vx = qx + 1;
					vy = qy;
				}
				if (i == 2 || i == 4) {
					vertex.texCoords = { 1.0f, 0.0f };
					vx = qx + 1;
					vy = qy + 1;
				}
				if (i == 5) {
					vertex.texCoords = { 0.0f, 0.0f };
					vx = qx;
					vy = qy + 1;
				}

				int v = vx + vy * (nbVertsX);

				// load corresponding generated position
				vertex.pos = positions[v];

				// adjust texture coordinates such that texture spans right amount of quads
				vertex.texCoords = vertex.texCoords * texSizePerQuad + texCoordsOffset;

				// load corresponding computed normal
				vertex.normal = normals[v];

				// set color
				vertex.color = diffuseColor;

				vertices.push_back(vertex);
			}
		}
	}
}

void GeometricObject::loadFromFile(const char* filename) {
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

			// Set color (white by default)
			vertex.color = diffuseColor;

			// Set texture coordinates
			vertex.texCoords = { 0.0f, 0.0f };

			vertices.push_back(vertex);
		}
	}
}

void GeometricObject::generateBufferObjects() {
	// generate VBO and VAO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	// bind VBO and VAO
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// specify total size of buffer object
	glBufferData(GL_ARRAY_BUFFER, size() * sizeof(Vertex), data(), GL_STATIC_DRAW);

	// specify position vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);

	// specify normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	// specify color vectors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(2);

	// specify texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(3);
}

std::vector<Vertex>* GeometricObject::getVertices() {
	return &vertices;
}

int GeometricObject::size() {
	return vertices.size();
}

Vertex* GeometricObject::data() {
	return vertices.data();
}

glm::mat4* GeometricObject::getModelMatrix() {
	if (parent) {
		completeModel = *parent->getModelMatrix() * model;
		return &completeModel;
	}
	else {
		return &model;
	}
}
void GeometricObject::setModelMatrix(glm::mat4 matrix) {
	model = matrix;
}
void GeometricObject::clearModelMatrix() {
	model = glm::mat4();
}
void GeometricObject::pushModelMatrix() {
	modelStack.push_back(model);
}
void GeometricObject::popModelMatrix() {
	loadModelMatrix();
	modelStack.pop_back();
}
void GeometricObject::loadModelMatrix() {
	model = modelStack.back();
}
void GeometricObject::applyPosition() {
	translate(position);
}

void GeometricObject::translate(glm::vec3 translation) {
	model = glm::translate(model, translation);
}
void GeometricObject::translateX(float translation) {
	translate(glm::vec3(translation, 0, 0));
}
void GeometricObject::translateY(float translation) {
	translate(glm::vec3(0, translation, 0));
}
void GeometricObject::translateZ(float translation) {
	translate(glm::vec3(0, 0, translation));
}

void GeometricObject::rotate(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}
void GeometricObject::rotateX(float angle) {
	rotate(angle, glm::vec3(1.0, 0, 0));
}
void GeometricObject::rotateY(float angle) {
	rotate(angle, glm::vec3(0, 1.0, 0));
}
void GeometricObject::rotateZ(float angle) {
	rotate(angle, glm::vec3(0, 0, 1.0));
}

void GeometricObject::scale(glm::vec3 factors) {
	model = glm::scale(model, factors);
}
void GeometricObject::scaleX(float factor) {
	scale(glm::vec3(factor, 0, 0));
}
void GeometricObject::scaleY(float factor) {
	scale(glm::vec3(0, factor, 0));
}
void GeometricObject::scaleZ(float factor) {
	scale(glm::vec3(0, 0, factor));
}
