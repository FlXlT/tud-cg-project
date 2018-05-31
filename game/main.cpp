// Library for OpenGL function loading
// Must be included before GLFW
#define GLEW_STATIC
#include <GL/glew.h>

// Library for window creation and event handling
#include <GLFW/glfw3.h>

// to transform vec4 and vec3 into strings
#include "glm/ext.hpp"

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

#define _USE_MATH_DEFINES
#include <math.h>

#include "vertex.h"

#include "scene.h"
#include "geometric_object.h"
#include "mesh.h"
#include "grid.h"

// Configuration
const int WIDTH = 800;
const int HEIGHT = 600;

float mouseXcoord = 0;
float mouseYcoord = 0;

glm::vec4 screenposSpaceship;
glm::vec4 screenposWeaponLeft;
glm::vec4 screenposWeaponRight;

namespace cameraModes {
	const unsigned int fixed = 0;
	const unsigned int follow = 1;
	const unsigned int side = 2;
}

// Scene
Scene scene;

int hitcount;

// Setup a set of cameras
unsigned int cameraMode = cameraModes::fixed;
glm::vec3 initialCamPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 initialCamUp = glm::vec3(0.0f, 1.0f, 0.0f);

std::vector<Camera> cameras;

Camera firstCamera;
Camera secondCamera;
Camera* mainCamera;

bool useTex = true;

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
	scene.handleKey(key, action);

	switch (key)
	{
	case GLFW_KEY_1:
		mainCamera = &cameras[0];
		break;
	case GLFW_KEY_2:
		mainCamera = &cameras[1];
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS && scene.terrain.targetSpeed.y < 0.0f) {
			scene.terrain.targetSpeed.y = 0.0f;
		}
		else if (action == GLFW_PRESS) {
			scene.terrain.targetSpeed.y = -0.02f;
		}
		break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS && cameraMode != cameraModes::follow) {
			cameraMode = cameraModes::follow;
			firstCamera.position = initialCamPos;
			firstCamera.up = initialCamUp;
			firstCamera.forward = -firstCamera.position;
		}
		else if (action == GLFW_PRESS) {
			cameraMode = cameraModes::fixed;
			firstCamera.position = initialCamPos;
			firstCamera.up = initialCamUp;
			firstCamera.forward = -firstCamera.position;
		}
		break;
	case GLFW_KEY_C:
		if (action == GLFW_PRESS && cameraMode != cameraModes::side) {
			cameraMode = cameraModes::side;
			firstCamera.position = glm::vec3(-15.0f, 0.0f, 0.0f);
			firstCamera.up = glm::vec3(0.0f, 0.0f, 1.0f);
			firstCamera.forward = -firstCamera.position;
		}
		else if (action == GLFW_PRESS) {
			cameraMode = cameraModes::fixed;
			firstCamera.position = initialCamPos;
			firstCamera.up = initialCamUp;
			firstCamera.forward = -firstCamera.position;
		}
		break;
	case GLFW_KEY_T:
		if (action == GLFW_PRESS && !useTex) {
			useTex = true;
		}
		else if (action == GLFW_PRESS) {
			useTex = false;
		}
		break;
	default:
		break;
	}
}

// Mouse button handle function
void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	camMouseButtonHandler(button, action);
	scene.sceneMouseButtonHandler(button, action, mouseXcoord, mouseYcoord);
}

void cursorPosHandler(GLFWwindow* window, double xpos, double ypos)
{
	scene.mouseProjection = glm::vec2(((xpos*10) / (WIDTH)) - 5, ((ypos * 10) / (HEIGHT)) - 5);
	mouseXcoord = xpos;
	mouseYcoord = ypos;
}


int main() {
	scene.build();

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create OpenGL context!" << std::endl;
		std::cout <<  "Press enter to close."; getchar();
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, keyboardHandler);
	glfwSetCursorPosCallback(window, cursorPosHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	
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
	stbi_uc* pixels = stbi_load("assets/textures/moon.jpg", &texwidth, &texheight, &texchannels, 3);
	//stbi_uc* pixels = stbi_load("smiley.png", &texwidth, &texheight, &texchannels, 3);

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

	scene.generateBufferObjects();

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
	firstCamera.aspect = WIDTH / (float)HEIGHT;
	firstCamera.position = initialCamPos;
	firstCamera.forward  = -firstCamera.position; // point to origin
	firstCamera.up = initialCamUp;
	cameras.push_back(firstCamera);

	/////////////////// Create second camera for shadow mapping
	secondCamera.aspect = WIDTH / (float)HEIGHT;
	secondCamera.position = glm::vec3(0, 0, 15.0f);
	secondCamera.forward  = -secondCamera.position;
	secondCamera.up = glm::vec3(-1.0f, 1.0f, 1.0f);
	cameras.push_back(secondCamera);

	// Assign the first camera as the main viewport
	// The other cameras are mainly for shadow mapping
	mainCamera = &firstCamera;

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glm::mat4 mvp = mainCamera->vpMatrix();
		glm::mat4 imvp = inverse(mvp);

		// mouse coords from -1 to 1
		const float mouseX = 2 * mouseXcoord / WIDTH - 1;
		const float mouseY = -(2 * mouseYcoord / HEIGHT - 1);

		glm::vec4 spaceshipScreenPos = mvp * glm::vec4(scene.spaceship.position, 1.0f);
		spaceshipScreenPos = glm::vec4(spaceshipScreenPos * glm::vec4(1.0f / spaceshipScreenPos.w));

		// mouse screen pos
		glm::vec4 mouseScreenPos = { mouseX, mouseY, spaceshipScreenPos.z, 1.0f };

		// mouse world pos
		glm::vec4 mouseWorldPos = imvp * mouseScreenPos;

		scene.cursor.position = glm::vec3(mouseWorldPos / glm::vec4(mouseWorldPos.w));

		scene.update();

		if (cameraMode == cameraModes::follow) {
			firstCamera.position = scene.spaceship.position + glm::vec3(0, 0, 2.0f);
			firstCamera.forward = scene.spaceship.position - firstCamera.position;
		}

		// Model/view/projection matrix from the point of view of the mainCamera and shadowCamera
		mvp = mainCamera->vpMatrix();
		glm::mat4 lightMVP = secondCamera.vpMatrix();

		float angleWeaponLeft = 0;
		float angleWeaponRight = 0;

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

			//// Set uniforms in fragment shader
			// Set projection matrix
			glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(lightMVP));
			glUniform3fv(glGetUniformLocation(shadowProgram, "viewPos"), 1, glm::value_ptr(secondCamera.position));
			glUniform1f(glGetUniformLocation(shadowProgram, "time"), static_cast<float>(glfwGetTime()));

			// Render objects
			std::vector<GeometricObject*> geometricObjects = scene.getGeometricObjects();
			//for (int i = 0; i < geometricObjects.size(); i++) {
			for (int i = 0; i < 7; i++) {
				
				GeometricObject obj = *geometricObjects[i];
				glBindVertexArray(obj.vao);
				if (i == 0) {
					glm::vec4 tempVec = mvp * glm::vec4(obj.position,1.0);
					scene.spaceshipProjection = glm::vec2(tempVec[0], tempVec[1]);
				}
				if (i == 1) {
					scene.spaceship.weaponLeft.updateAngle(mvp, obj, scene.mouseProjection[0], scene.mouseProjection[1]);
					obj.rotateY(scene.spaceship.weaponLeft.angle);
				}
				if (i == 3) {
					scene.spaceship.weaponRight.updateAngle(mvp, obj, scene.mouseProjection[0], scene.mouseProjection[1]);
					obj.rotateY(scene.spaceship.weaponRight.angle);
				}

				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(lightMVP * *obj.getModelMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
				glDrawArrays(GL_TRIANGLES, 0, obj.size());
			}

			GeometricObject obj = *geometricObjects[11];
			glBindVertexArray(obj.vao);
			glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(lightMVP * *obj.getModelMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
			glDrawArrays(GL_TRIANGLES, 0, obj.size());

			hitcount = scene.hitcount;

			// Draw the bawsman
			if (hitcount < 3) {
				GeometricObject obj = *geometricObjects[8 + hitcount];
				glBindVertexArray(obj.vao);
				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(lightMVP * *obj.getModelMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
				glDrawArrays(GL_TRIANGLES, 0, obj.size());
			}

			// Unbind the off-screen framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Bind the shader
		glUseProgram(mainProgram);
		updateCamera(*mainCamera);

		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform3fv(glGetUniformLocation(mainProgram, "viewPos"), 1, glm::value_ptr(mainCamera->position)); // Set view
		glUniform1f(glGetUniformLocation(mainProgram, "time"), static_cast<float>(glfwGetTime())); // Expose current time in shader uniform

		// Set MVP from the perspective of the shadow camera
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "lightMVP"), 1, GL_FALSE, glm::value_ptr(lightMVP));
		glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(secondCamera.position));

		// Bind the shadow map to texture slot 0
		GLint texture_unit = 0;

		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, texShadow);
		glUniform1i(glGetUniformLocation(mainProgram, "texShadow"), texture_unit);

		// Bind the material map map to texture slot 1
		texture_unit = 1;

		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, texLight);
		glUniform1i(glGetUniformLocation(mainProgram, "texMaterial"), texture_unit);

		GLint modelLoc = glGetUniformLocation(mainProgram, "model");

		// Set viewport size
		glViewport(0, 0, WIDTH, HEIGHT);

		// Clear the framebuffer to black and depth to maximum value
		glClearDepth(1.0f);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		// Render objects
		std::vector<GeometricObject*> geometricObjects = scene.getGeometricObjects();
		//for (int i = 0; i < geometricObjects.size(); i++) {
		for (int i = 0; i < 7; i++) {
			GeometricObject obj = *geometricObjects[i];
			glBindVertexArray(obj.vao);

			//Get rotation for weapons computed in the shadowPrograw
			if (i == 1) {
				obj.rotateY(scene.spaceship.weaponLeft.angle);
			}
			if (i == 3) {
				obj.rotateY(scene.spaceship.weaponRight.angle);
			}

			glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp * *obj.getModelMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(mainProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
			glUniform3fv(glGetUniformLocation(mainProgram, "specularColor"), 1, glm::value_ptr(obj.specularColor));
			glUniform1f(glGetUniformLocation(mainProgram, "specularIntensity"), obj.specularIntensity);
			glUniform1i(glGetUniformLocation(mainProgram, "useTexMaterial"), obj.useTex && useTex);
		
			glDrawArrays(GL_TRIANGLES, 0, obj.size());
		}

		GeometricObject obj = *geometricObjects[11];
		glBindVertexArray(obj.vao);

		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp * *obj.getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
		glUniform3fv(glGetUniformLocation(mainProgram, "specularColor"), 1, glm::value_ptr(obj.specularColor));
		glUniform1f(glGetUniformLocation(mainProgram, "specularIntensity"), obj.specularIntensity);
		glUniform1i(glGetUniformLocation(mainProgram, "useTexMaterial"), obj.useTex);

		glDrawArrays(GL_TRIANGLES, 0, obj.size());


		hitcount = scene.hitcount;

		// Draw the bawsman
		if (hitcount < 3) {
			GeometricObject obj = *geometricObjects[8 + hitcount];
			glBindVertexArray(obj.vao);

			obj.translateZ(3);
			obj.translateY(2);
			obj.rotateX(45 * atan(1) * 4 / 180);
			obj.rotateY(atan(1) * 4 / 180);

			glUniformMatrix4fv(glGetUniformLocation(mainProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp * *obj.getModelMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(mainProgram, "model"), 1, GL_FALSE, glm::value_ptr(*obj.getModelMatrix()));
			glUniform3fv(glGetUniformLocation(mainProgram, "specularColor"), 1, glm::value_ptr(obj.specularColor));
			glUniform1f(glGetUniformLocation(mainProgram, "specularIntensity"), obj.specularIntensity);
			glUniform1i(glGetUniformLocation(mainProgram, "useTexMaterial"), obj.useTex);

			glDrawArrays(GL_TRIANGLES, 0, obj.size());
		}

		// Present result to the screen
		glfwSwapBuffers(window);
	}

	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texShadow);
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}
