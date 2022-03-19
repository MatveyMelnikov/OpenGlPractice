#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.h"
#include "ResourceManager.h"
#include "Network.h"

const glm::ivec2 WINDOW_SIZE = glm::ivec2(640, 480);
const std::string RESOURCES_PATH = "../res/";

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	// Rendering during screen resizing
	Engine* engine = Engine::getInstance(window);
	engine->render();
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_2) {
		printf("Update network...\n");
		Network::getInstance(window, RESOURCES_PATH + "network.json")->updateNetwork();
		Engine::getInstance(window)->loadConfigFile("../res/config.json");
	}
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cerr << "ERROR: GLFW wasnt create!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Network", NULL, NULL);
	if (!window)
	{
		std::cerr << "ERROR: Window wasnt create!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cerr << "ERROR: Glad wasnt init!" << std::endl;
		return -1;
	}

	glEnable(GL_MULTISAMPLE);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	Engine* engine = Engine::getInstance(window);
	ResourceManager* resourceManager = ResourceManager::getInstance();
	resourceManager->createShaderProgram(RESOURCES_PATH);
	Network* network = Network::getInstance(window, RESOURCES_PATH + "network.json");

	// After creating elements
	engine->loadConfigFile(RESOURCES_PATH + "config.json");

	while (!glfwWindowShouldClose(window))
	{
		engine->render();
	}

	glfwTerminate();
	return 0;
}