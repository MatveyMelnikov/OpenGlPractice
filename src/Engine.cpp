#include "Engine.h"
Engine* Engine::instance_ = nullptr;

Engine* Engine::getInstance(GLFWAPI::GLFWwindow* window) {
	if (instance_ == nullptr)
		instance_ = new Engine(window);
	return instance_;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS)
		Engine::getInstance(window)->setKeyStatus(key, true);
	else if (action == GLFW_RELEASE)
		Engine::getInstance(window)->setKeyStatus(key, false);
}

Engine::Engine(GLFWAPI::GLFWwindow* window) {
	window_ = window;
	glfwSetKeyCallback(window_, glfwKeyCallback);
	std::fill_n(&keys_[0], sizeof(keys_) / sizeof(bool), false);
}

void Engine::moveCamera(glm::fvec2 cameraPosition) {
	cameraPosition_ += cameraPosition;
}

void Engine::setCameraPosition(glm::fvec2 cameraPosition) {
	cameraPosition_ = glm::fvec2(
		cameraPosition.x,
		cameraPosition.y
	);
}

void Engine::setCameraScale(float scale) {
	scale_ = scale;
}

float Engine::getCameraScale() {
	return scale_;
}

void Engine::setCameraSpeed(float speed) {
	cameraSpeed_ = speed;
}

float Engine::getCameraSpeed() {
	return cameraSpeed_;
}

float Engine::getLastFrameTime() {
	return lastFrameTime_;
}

void Engine::setKeyStatus(int key, bool status) {
	keys_[key] = status;
}

bool Engine::getKeyStatus(int key) {
	if (key >= 0 && key < 1024)
		return keys_[key];
	else 
		false;
}

void Engine::catchMovementKeys() {
	float deltaTime = glfwGetTime() - lastFrameTime_;
	float speed = cameraSpeed_ * deltaTime;
	if (keys_[GLFW_KEY_W])
		moveCamera(glm::fvec2(0.f, speed));
	if (keys_[GLFW_KEY_S])
		moveCamera(glm::fvec2(0.f, -speed));
	if (keys_[GLFW_KEY_A])
		moveCamera(glm::fvec2(-speed, 0.f));
	if (keys_[GLFW_KEY_D])
		moveCamera(glm::fvec2(speed, 0.f));
	if (keys_[GLFW_KEY_E]) {
		if (scale_ > 0.1f)
			scale_ -= 1.f * deltaTime;
	} if (keys_[GLFW_KEY_Q]) {
		if (scale_ < 3.0f)
			scale_ += 1.f * deltaTime;
	}
}

void Engine::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glm::ivec2 windowSize;
	glfwGetFramebufferSize(window_, &windowSize.x, &windowSize.y);
	glm::fvec2 cameraOffset_ = windowSize / 2;
	glViewport(0, 0, windowSize.x, windowSize.y);

	glm::mat4 projectionMatrix = glm::ortho(
		0.f, 
		(float)windowSize.x * scale_, 
		0.f, 
		(float)windowSize.y * scale_, 
		0.f, 
		10.f
	);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(
			cameraPosition_.x - cameraOffset_.x * scale_,
			cameraPosition_.y - cameraOffset_.y * scale_,
			1.f
		),
		glm::vec3(
			cameraPosition_.x - cameraOffset_.x * scale_,
			cameraPosition_.y - cameraOffset_.y * scale_,
			0.f
		),
		glm::vec3(0.f, 1.f, 0.f)
	);
	glm::mat4 resultMatrix = projectionMatrix * viewMatrix;

	ResourceManager* resourceManager = ResourceManager::getInstance();
	for (unsigned int i = 0; i < ResourceManager::getInstance()->getLinesAmount(); i++) {
		resourceManager->getLine(i)->getShader()->setUniform("viewMatrix", resultMatrix);
		resourceManager->getLine(i)->render(windowSize);
	}
	for (unsigned int i = 0; i < ResourceManager::getInstance()->getCirclesAmount(); i++) {
		resourceManager->getCircle(i)->getShader()->setUniform("viewMatrix", resultMatrix);
		resourceManager->getCircle(i)->render(windowSize);
	}

	glfwSwapBuffers(window_);

	glfwPollEvents();
	catchMovementKeys();

	lastFrameTime_ = glfwGetTime();
}
