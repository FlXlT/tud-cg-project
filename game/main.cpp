// Library for OpenGL function loading
// Must be included before GLFW
#define GLEW_STATIC
#include <GL/glew.h>

// Library for window creation and event handling
#include <GLFW/glfw3.h>

// Library for vertex and matrix math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Library for loading .OBJ model
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// Library for loading an image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Header for camera structure/functions
#include "camera.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <math.h>

#include "vertex.h"
#include "object.h"

// Configuration
const int WIDTH = 800;
const int HEIGHT = 600;

float weaponRot = 0.0;
float weaponLeftRot = 0.0;
float weaponRightRot = 0.0;

// Setup a set of cameras
std::vector<Camera> cameras;
// Initialize mainCamera which is visible for the user.
Camera mainCamera;

// Helper function to read a file like a shader
std::string readFile(const std::string& path) {
	std::ifstream file(path, std::ios::binary);

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

bool checkShaderErrors(GLuint shader) {
	// Check if the shader compiled successfully
	GLint compileSuccessful;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccessful);

	// If it didn't, then read and print the compile log
	if (!compileSuccessful) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> logBuffer(logLength);
		glGetShaderInfoLog(shader, logLength, nullptr, logBuffer.data());

		std::cerr << logBuffer.data() << std::endl;

		return false;
	} else {
		return true;
	}
}

bool checkProgramErrors(GLuint program) {
	// Check if the program linked successfully
	GLint linkSuccessful;
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccessful);

	// If it didn't, then read and print the link log
	if (!linkSuccessful) {
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> logBuffer(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());

		std::cerr << logBuffer.data() << std::endl;

		return false;
	} else {
		return true;
	}
}

// OpenGL debug callback
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
		std::cerr << "OpenGL: " << message << std::endl;
	}
}


// Key handle function
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cameraKeyboardHandler(key, action);

	switch (key)
	{
	case GLFW_KEY_1:
		mainCamera = cameras[0];
		break;
	case GLFW_KEY_2:
		mainCamera = cameras[1];
		break;
	default:
		break;
	}
}

// Mouse button handle function
void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	camMouseButtonHandler(button, action);
}

void cursorPosHandler(GLFWwindow* window, double xpos, double ypos)
{
	weaponRot = (float) atan((xpos - WIDTH/2) / (ypos - HEIGHT/2));
	weaponLeftRot = (float)-1*atan((xpos - (WIDTH  / 2 + WIDTH * 0.075)) / (ypos - HEIGHT / 2));
	weaponRightRot = (float)-1*atan((xpos - (WIDTH / 2 - WIDTH * 0.075)) / (ypos - HEIGHT / 2));
	//camCursorPosHandler(xpos, ypos);
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return EXIT_FAILURE;
	}

	//////////////////// Create window and OpenGL 4.3 debug context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shadow mapping practical", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create OpenGL context!" << std::endl;
		std::cout <<  "Press enter to close."; getchar();
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, keyboardHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetCursorPosCallback(window, cursorPosHandler);

	// Activate the OpenGL context
	glfwMakeContextCurrent(window);

	// Initialize GLEW extension loader
	glewExperimental = GL_TRUE;
	glewInit();

	// Set up OpenGL debug callback
	glDebugMessageCallback(debugCallback, nullptr);

	GLuint mainProgram = glCreateProgram();
	GLuint shadowProgram = glCreateProgram();


	////////////////// Load and compile main shader program
	{
		std::string vertexShaderCode = readFile("shader.vert");
		const char* vertexShaderCodePtr = vertexShaderCode.data();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCodePtr, nullptr);
		glCompileShader(vertexShader);

		std::string fragmentShaderCode = readFile("shader.frag");
		const char* fragmentShaderCodePtr = fragmentShaderCode.data();

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCodePtr, nullptr);
		glCompileShader(fragmentShader);

		if (!checkShaderErrors(vertexShader) || !checkShaderErrors(fragmentShader)) {
			std::cerr << "Shader(s) failed to compile!" << std::endl;
			std::cout << "Press enter to close."; getchar();
			return EXIT_FAILURE;
		}

		// Combine vertex and fragment shaders into single shader program
		glAttachShader(mainProgram, vertexShader);
		glAttachShader(mainProgram, fragmentShader);
		glLinkProgram(mainProgram);

		if (!checkProgramErrors(mainProgram)) {
			std::cerr << "Main program failed to link!" << std::endl;
			std::cout << "Press enter to close."; getchar();
			return EXIT_FAILURE;
		}
	}

	////////////////// Load and compile shadow shader program
	{
		std::string vertexShaderCode = readFile("shadow.vert");
		const char* vertexShaderCodePtr = vertexShaderCode.data();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCodePtr, nullptr);
		glCompileShader(vertexShader);

		std::string fragmentShaderCode = readFile("shadow.frag");
		const char* fragmentShaderCodePtr = fragmentShaderCode.data();

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCodePtr, nullptr);
		glCompileShader(fragmentShader);

		if (!checkShaderErrors(vertexShader) || !checkShaderErrors(fragmentShader)) {
			std::cerr << "Shader(s) failed to compile!" << std::endl;
			return EXIT_FAILURE;
		}

		// Combine vertex and fragment shaders into single shader program
		glAttachShader(shadowProgram, vertexShader);
		glAttachShader(shadowProgram, fragmentShader);
		glLinkProgram(shadowProgram);

		if (!checkProgramErrors(shadowProgram)) {
			std::cerr << "Shadow program failed to link!" << std::endl;
			return EXIT_FAILURE;
		}
	}

	// Create Texture
	int texwidth, texheight, texchannels;
	stbi_uc* pixels = stbi_load("smiley.png", &texwidth, &texheight, &texchannels, 3);

	GLuint texLight;
	glGenTextures(1, &texLight);
	glBindTexture(GL_TEXTURE_2D, texLight);

	// Upload pixels into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Set behaviour for when texture coordinates are outside the [0, 1] range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	////////////////////////// Load vertices of model

	Object spaceship;
	spaceship.loadFromFile("spaceship.obj");


	//// WEAPON MODEL LOAD
	tinyobj::attrib_t attrib2;
	std::vector<tinyobj::shape_t> shapes2;
	std::vector<tinyobj::material_t> materials2;
	std::string err2;

	if (!tinyobj::LoadObj(&attrib2, &shapes2, &materials2, &err2, "weapons.obj")) {
		std::cerr << err2 << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Vertex> vertices2;

	// read triangle vertices from obj file
	for (const auto& shape : shapes2) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			// retrieve coordinates for vertex by index
			vertex.pos = {
				attrib2.vertices[3 * index.vertex_index + 0],
				attrib2.vertices[3 * index.vertex_index + 1],
				attrib2.vertices[3 * index.vertex_index + 2]
			};

			// retrieve components of normal by index
			vertex.normal = {
				attrib2.normals[3 * index.normal_index + 0],
				attrib2.normals[3 * index.normal_index + 1],
				attrib2.normals[3 * index.normal_index + 2]
			};

			vertices2.push_back(vertex);
		}
	}

	//// SINGLE WEAPON MODEL LOAD
	tinyobj::attrib_t attrib3;
	std::vector<tinyobj::shape_t> shapes3;
	std::vector<tinyobj::material_t> materials3;
	std::string err3;

	if (!tinyobj::LoadObj(&attrib3, &shapes3, &materials3, &err3, "SingleWeapon.obj")) {
		std::cerr << err3 << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Vertex> vertices3;

	// read triangle vertices from obj file
	for (const auto& shape : shapes3) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			// retrieve coordinates for vertex by index
			vertex.pos = {
				attrib3.vertices[3 * index.vertex_index + 0],
				attrib3.vertices[3 * index.vertex_index + 1],
				attrib3.vertices[3 * index.vertex_index + 2]
			};

			// retrieve components of normal by index
			vertex.normal = {
				attrib3.normals[3 * index.normal_index + 0],
				attrib3.normals[3 * index.normal_index + 1],
				attrib3.normals[3 * index.normal_index + 2]
			};

			vertices3.push_back(vertex);
		}
	}

	// World space positions of the models
	glm::vec3 modelPositions[] = {
		glm::vec3( 0.0f,  -0.5f,  0.0f),     // Spaceship location
		glm::vec3( 0.0f,   0.0f,  0.0f),
		glm::vec3(-0.125f, -0.42f, 0.0f),	   // Left weapon from spaceship
		glm::vec3( 0.125f, -0.42f, 0.0f)	   // Right weapon from spaceship
	};

	//////////////////// Create Vertex Buffer Objects
	GLuint vao[3], vbo[3];
	glGenVertexArrays(3, vao); // Bind vertex data to shader inputs using their index (location)
	glGenBuffers(3, vbo);

	////// SPACESHIP
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, (*spaceship.getVertices()).size() * sizeof(Vertex), (*spaceship.getVertices()).data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // position vectors
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	//////// WEAPON
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(Vertex), vertices2.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // The position vectors
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // The normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	//////// SINGLE WEAPON
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(Vertex), vertices3.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // The position vectors
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // The normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	//////////////////// Create Shadow Texture
	GLuint texShadow;
	const int SHADOWTEX_WIDTH  = 1024;
	const int SHADOWTEX_HEIGHT = 1024;
	glGenTextures(1, &texShadow);
	glBindTexture(GL_TEXTURE_2D, texShadow);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Set behaviour for when texture coordinates are outside the [0, 1] range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set interpolation for texture sampling (GL_NEAREST for no interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//////////////////// Create framebuffer for extra texture
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);

	/////////////////// Set shadow texure as depth buffer for this framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texShadow, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/////////////////// Create main camera
	Camera firstCamera;
	firstCamera.aspect = WIDTH / (float)HEIGHT;
	mainCamera.position = glm::vec3(0.0f, 0.0f, 3.0f);
	mainCamera.forward  = glm::vec3(0.0f, 0.0f, 0.0f);
	cameras.push_back(firstCamera);

	/////////////////// Create second camera for shadow mapping
	Camera secondCamera;
	secondCamera.aspect = WIDTH / (float)HEIGHT;
	mainCamera.position = glm::vec3(0.0f, 0.0f, 4.0f);
	mainCamera.forward  = glm::vec3(0.0f, 0.0f, 0.0f);
	cameras.push_back(secondCamera);

	// Assign the first camera as the main viewport
	// The other cameras are mainly for shadow mapping
	mainCamera = firstCamera;

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Model/view/projection matrix from the point of view of the shadowCamera
		glm::mat4 lightMVP = secondCamera.vpMatrix();

		////////// Stub code for you to fill in order to render the shadow map
		{
			// Bind the off-screen framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			// Clear the shadow map and set needed options
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			glUseProgram(shadowProgram); // Bind the shader
			glViewport(0, 0, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT); // Set viewport size

			// Execute draw command
			/*glDrawArrays(GL_TRIANGLES, 0, vertices.size());*/

			// .... HERE YOU MUST ADD THE CORRECT UNIFORMS FOR RENDERING THE SHADOW MAP

			glBindVertexArray(vao[0]); // Bind vertex data
			glDrawArrays(GL_TRIANGLES, 0, (*spaceship.getVertices()).size()); // Execute draw command
			glBindVertexArray(vao[1]); // Bind vertex data
			glDrawArrays(GL_TRIANGLES, 0, vertices2.size()); // Execute draw command

			//// Set uniforms in fragment shader
			// Set projection matrix
			glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(lightMVP));

			// Set view position
			glUniform3fv(glGetUniformLocation(shadowProgram, "viewPos"), 1, glm::value_ptr(secondCamera.position));

			// Expose current time in shader uniform
			glUniform1f(glGetUniformLocation(shadowProgram, "time"), static_cast<float>(glfwGetTime()));

			// Bind vertex data
			glBindVertexArray(vao[0]);

			// Execute draw command
			glDrawArrays(GL_TRIANGLES, 0, (*spaceship.getVertices()).size());

			// Unbind the off-screen framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Bind the shader
		glUseProgram(mainProgram);
		updateCamera(mainCamera);
		glm::mat4 mvp = mainCamera.vpMatrix();

		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform3fv(glGetUniformLocation(mainProgram, "viewPos"), 1, glm::value_ptr(mainCamera.position)); // Set view
		glUniform1f(glGetUniformLocation(mainProgram, "time"), static_cast<float>(glfwGetTime())); // Expose current time in shader uniform

		// Set view position
		glUniform3fv(glGetUniformLocation(mainProgram, "viewPos"), 1, glm::value_ptr(mainCamera.position));

		// Expose current time in shader uniform
		glUniform1f(glGetUniformLocation(mainProgram, "time"), static_cast<float>(glfwGetTime()));

		// Set MVP from the perspective of the shadow camera
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "lightMVP"), 1, GL_FALSE, glm::value_ptr(lightMVP));

		// Bind vertex data
		//glBindVertexArray(vao);

		// Bind the shadow map to texture slot 0
		GLint texture_unit = 0;

		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, texShadow);
		glUniform1i(glGetUniformLocation(mainProgram, "texShadow"), texture_unit);

		GLint modelLoc = glGetUniformLocation(mainProgram, "model");

		// Set viewport size
		glViewport(0, 0, WIDTH, HEIGHT);

		// Clear the framebuffer to black and depth to maximum value
		glClearDepth(1.0f);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(vao[0]); // Bind vertex data


		float angleX = -90 * atan(1) * 4 / 180;
		float angleY = 180 * atan(1) * 4 / 180;

		spaceship.clearModelMatrix();
		spaceship.translate(modelPositions[0]);
		spaceship.rotateX(angleX);
		spaceship.rotateY(angleY);
		spaceship.scale(glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(*spaceship.getModelMatrix()));
		glDrawArrays(GL_TRIANGLES, 0, (*spaceship.getVertices()).size());		// Execute draw commands

		glm::mat4 model;

		glBindVertexArray(vao[2]);
		model = glm::mat4();
		model = glm::translate(model, modelPositions[2]);
		model = glm::rotate(model, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, weaponRightRot, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

		model = glm::mat4();
		model = glm::translate(model, modelPositions[3]);
		model = glm::rotate(model, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, weaponLeftRot, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

		// Present result to the screen
		glfwSwapBuffers(window);
	}

	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texShadow);
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}
