#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "ResourceManager.h"
#include "Engine.h"

class Network {
public:
	Network(GLFWAPI::GLFWwindow* window, const std::string& networkFilePath);
	void updateNetwork();
private:
	void createCircles();
	void createLines();
private:
	std::string networkFilePath_;
	std::vector<std::pair<int, glm::fvec3>> layers_; // Number of circles on layer, and their color
	Engine* engine_ = nullptr;
	ResourceManager* resourceManager_ = nullptr;
};