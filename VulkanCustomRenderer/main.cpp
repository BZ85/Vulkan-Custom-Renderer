// Author: Xinjie Zhu

#define GLFW_INCLUDE_VULKAN
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
//#include <vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <stdexcept>

#include <iostream>
#include "vulkanRenderer.h"


GLFWwindow * window;
VulkanRenderer vulkanRenderer;

bool mouseDragging = false;
bool isLeftMouseButton = false;
bool isRightMouseButton = false;
bool autoRotation = true;

double lastMouseX;
double lastMouseY;
double currentMouseX;
double currentMouseY;

float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleZ = 0.0f;

void initWindow(std::string wName = "test window", const int width = 800, const int height = 600) 
{

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	




}

void rotateObject(double deltaX, double deltaY) {

	glm::mat4 transform(1.0f);

	// translate the object to the origin (center of screen)
	transform = glm::translate(transform, glm::vec3(-1.0f, 0.0f, -1.0f));

	// right mouse button control the rotation angle around Y axis
	// dragging horitzontally
	if(isRightMouseButton) rotationAngleY += deltaX;

	// left mouse button control the rotation angle around X axis and Z axis
	// dragging vertically for X axis and horizontally for Y axis
	if (isLeftMouseButton) {
		rotationAngleX += deltaY;
		rotationAngleZ += deltaX;
	}

	if (rotationAngleX > 360.0f) { rotationAngleX -= 360.0f; }
	if (rotationAngleY > 360.0f) { rotationAngleY -= 360.0f; }
	if (rotationAngleZ > 360.0f) { rotationAngleZ -= 360.0f; }
	
	transform = glm::rotate(transform, glm::radians(static_cast<float>(rotationAngleY)), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(static_cast<float>(rotationAngleX)), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(static_cast<float>(rotationAngleZ)), glm::vec3(0.0f, 0.0f, 1.0f));

	
	
	
	vulkanRenderer.updateModel(1, transform);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	//std::cout << "Mouse moved to: " << xpos << ", " << ypos << std::endl;

	if (mouseDragging) {
		currentMouseX = xpos;
		currentMouseY = ypos;

		// Compute delta movement
		double deltaX = currentMouseX - lastMouseX;
		double deltaY = currentMouseY - lastMouseY;

		// Apply transformations (e.g., rotation, translation)
		rotateObject(deltaX, deltaY);

		// Update last position
		lastMouseX = currentMouseX;
		lastMouseY = currentMouseY;
	}

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	
	// left mouse button
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			mouseDragging = true;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY); // Record initial position
			isLeftMouseButton = true;
		}
		else if (action == GLFW_RELEASE) {
			mouseDragging = false;
			isLeftMouseButton = false;
		}
	}

	// right mouse button
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			mouseDragging = true;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY); 
			isRightMouseButton = true;

		}
		else if (action == GLFW_RELEASE) {
			mouseDragging = false;
			isRightMouseButton = false;
		}
	}


	if (button == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			autoRotation = !autoRotation;
			std::cout << "aaa" << std::endl;
		}
	}
}

// Define the key callback function
// Press the space key to start or stop the auto rotation of the craft model
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	

	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			autoRotation = !autoRotation;
			
		}
	}

}

int main() {

	initWindow("Xinjie Zhu's Vulkan Renderer", 1920, 1080);

	if (vulkanRenderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetKeyCallback(window, keyCallback);

	//int seahawk = vulkanRenderer.createMeshModel("Models/Seahawk.obj");
	int aircraft = vulkanRenderer.createMeshModel("Models/E 45 Aircraft_obj.obj");

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	// Loop until closed
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		angle += 50.0f * deltaTime;
		if (angle > 360.0f) { angle -= 360.0f; }

		glm::mat4 firstModel(1.0f);
		glm::mat4 secondModel(1.0f);

		firstModel = glm::translate(firstModel, glm::vec3(-1.0f, 0.0f, -1.0f));
		//firstModel = glm::rotate(firstModel, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		firstModel = glm::rotate(firstModel, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		secondModel = glm::translate(secondModel, glm::vec3(0.0f, 0.0f, -5.0f));
		secondModel = glm::rotate(secondModel, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		secondModel = glm::translate(secondModel, glm::vec3(0.0f, 0.0f, 5.0f));
		
		
		if (autoRotation) {
			vulkanRenderer.updateModel(1, firstModel);

			// clear the rotation angle for mouse control
		    rotationAngleX = 0.0f;
		    rotationAngleY = 0.0f;
			rotationAngleZ = 0.0f;

		}
		//vulkanRenderer.updateModel(0, secondModel);
	

		vulkanRenderer.draw();
	}


	vulkanRenderer.cleanup();

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}