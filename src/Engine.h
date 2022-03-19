#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "glm/glm.hpp"
#include "ResourceManager.h"

class Engine {
public:
	static Engine* getInstance(GLFWAPI::GLFWwindow* window);
	void moveCamera(glm::fvec2 cameraPosition);
	void setCameraPosition(glm::fvec2 cameraPosition);
	void setCameraScale(float scale);
	float getCameraScale();
	void setCameraSpeed(float speed);
	float getCameraSpeed();
	void setScaleSpeed(float speed);
	float getLastFrameTime();
	void setKeyStatus(int key, bool status);
	bool getKeyStatus(int key);
	void catchMovementKeys();
	void loadConfigFile(const std::string& path);
	void render();
private:
	Engine(GLFWAPI::GLFWwindow* window);
	Engine(const Engine&) {};
	Engine& operator=(Engine&) = delete;
private:
	static Engine* instance_;
	GLFWAPI::GLFWwindow* window_;
	glm::fvec2 cameraPosition_ = glm::fvec2(0, 0);
	float scale_ = 1.0f;
	float lastFrameTime_ = 0.f;
	bool keys_[512];
	float cameraSpeed_ = 150.f;
	float scaleSpeed_ = 1.f;
};
