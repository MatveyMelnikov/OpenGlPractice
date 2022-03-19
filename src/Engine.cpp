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

void Engine::setScaleSpeed(float speed) {
	if (speed > 0)
		scaleSpeed_ = speed;
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
		return false;
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
			scale_ -= scaleSpeed_ * deltaTime;
	} if (keys_[GLFW_KEY_Q]) {
		if (scale_ < 3.0f)
			scale_ += scaleSpeed_ * deltaTime;
	}
}

void Engine::loadConfigFile(const std::string& path) {
	ResourceManager* resourceManager = ResourceManager::getInstance();
	std::string source = resourceManager->loadStringFromFile(path);
	if (source.empty())
		return;

	rapidjson::Document document;
	rapidjson::ParseResult result = document.Parse(source.c_str());
	if (result.IsError()) {
		printf(
			"-Parse error in %s: %s (%zu)\n",
			path.c_str(),
			rapidjson::GetParseError_En(result.Code()),
			result.Offset()
		);
		return;
	}

	int width = 0.f, radius = 0.f, precision = 0;
	glm::fvec3 linesColor;
	bool isWidth = false, isRadius = false, isColor = false;
	if (document.HasMember("cameraSpeed") && document["cameraSpeed"].IsFloat())
		setCameraSpeed(document["cameraSpeed"].GetFloat());
	if (document.HasMember("scaleSpeed") && document["scaleSpeed"].IsFloat())
		setScaleSpeed(document["scaleSpeed"].GetFloat());
	if (document.HasMember("linesWidth") && document["linesWidth"].IsInt()) {
		width = document["linesWidth"].GetInt();
		isWidth = true;
	}
	if (document.HasMember("circlesRadius") && document["circlesRadius"].IsInt()) {
		radius = document["circlesRadius"].GetInt();
		isRadius = true;
	}
	if (document.HasMember("circlesPrecision") && document["circlesPrecision"].IsInt())
		precision = document["circlesPrecision"].GetInt();
	if (document.HasMember("linesColor") && document["linesColor"].IsArray()) {
		auto color = document["linesColor"].GetArray();
		if (color.Size() == 3) {
			linesColor = glm::fvec3(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat());
			isColor = true;
		}
	}

	for (unsigned int i = 0; i < resourceManager->getLinesAmount(); i++) {
		if (isWidth)
			resourceManager->getLine(i)->setWidth(width);
		if (isColor)
			resourceManager->getLine(i)->setColor(linesColor);
	}
	for (unsigned int i = 0; i < resourceManager->getCirclesAmount(); i++) {
		if (precision >= 3)
			resourceManager->getCircle(i)->setPrecision((unsigned int)precision);
		if (isRadius)
			resourceManager->getCircle(i)->setRadius(radius);
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
