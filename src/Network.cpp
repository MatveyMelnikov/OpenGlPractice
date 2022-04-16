#include "Network.h"
Network* Network::instance_ = nullptr;

Network* Network::getInstance(GLFWAPI::GLFWwindow* window, const std::string& networkFilePath) {
	if (instance_ == nullptr)
		instance_ = new Network(window, networkFilePath);
	return instance_;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	Engine* engine = Engine::getInstance(window);
	ResourceManager* resourceManager = ResourceManager::getInstance();
	Network* network = Network::getInstance(window, RESOURCES_PATH + "network.json");
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_2) {
		printf("Update network...\n");
		network->updateNetwork();
		engine->loadConfigFile(RESOURCES_PATH + "config.json");
	}
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
		glm::ivec2 cursor = engine->getLocalCursorPosition();

		for (unsigned int i = 0; i < resourceManager->getCirclesAmount(); i++) {
			std::shared_ptr<Circle> circle = resourceManager->getCircle(i);
			if (
				pow(cursor.x - circle->getPosition().x, 2) +
				pow(cursor.y - circle->getPosition().y, 2) <=
				pow(circle->getRadius() * 2, 2)
				) { // circle equation
				engine->selectCircle(circle);
				break;
			}
		}
		network->setLeftMouseButtonStatus(true);
	}
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_1) {
		engine->selectCircle(nullptr);
		network->setLeftMouseButtonStatus(false);
	}
}

void Network::createCircles() {
	for (unsigned int i = 0; i < layers_.size(); i++) {
		unsigned int offsetY = 0;

		if (layers_[i].first % 2 == 0)
			offsetY = 50 * (layers_[i].first / 2) - 25;
		else
			offsetY = 50 * (layers_[i].first / 2);
		for (unsigned int j = 0; j < layers_[i].first; j++) {
			resourceManager_->createCircle(
				glm::ivec2(100 * i, offsetY - (50 * j)),
				layers_[i].second,
				0
			);
		}
	}
}

void Network::setLeftMouseButtonStatus(bool isPressed) {
	isLeftMouseButtonPressed_ = isPressed;
}

void Network::createLines() {
	unsigned int offset = 0; // Before startPoint
	for (unsigned int i = 0; i < layers_.size() - 1; i++) {
		for (unsigned int j = 0; j < layers_[i].first; j++) { // startPoint
			for (unsigned int k = 0; k < layers_[i + 1].first; k++) { // endPoint
				resourceManager_->createLine(
					resourceManager_->getCircle(j + offset)->getPosition(),
					resourceManager_->getCircle(k + offset + layers_[i].first)->getPosition(),
					glm::fvec3(0.3f, 0.3f, 0.3f) + glm::fvec3(0.1f * j, 0.1f * j, 0.1f * j),
					0
				);
			}
		}
		offset += layers_[i].first;
	}
}

void Network::updateLines() {
	if (isLeftMouseButtonPressed_) {
		ResourceManager::getInstance()->clearLines();
		createLines();
	}
}

Network::Network(GLFWAPI::GLFWwindow* window, const std::string& networkFilePath) {
	engine_ = Engine::getInstance(window);
	resourceManager_ = ResourceManager::getInstance();
	networkFilePath_ = networkFilePath;
	glfwSetMouseButtonCallback(window, mouseCallback);
	updateNetwork();
}

void Network::updateNetwork() {
	if (engine_ == nullptr)
		return;
	unsigned int layersSize = layers_.size();
	layers_.clear();

	std::string source = resourceManager_->loadStringFromFile(networkFilePath_);
	if (source.empty())
		return;

	rapidjson::Document document;
	rapidjson::ParseResult result = document.Parse(source.c_str());
	if (result.IsError()) {
		printf(
			"-Parse error in %s: %s (%zu)\n",
			networkFilePath_.c_str(),
			rapidjson::GetParseError_En(result.Code()),
			result.Offset()
		);
		return;
	}

	if (document.HasMember("layers") && document["layers"].IsArray()) {
		auto layersArray = document["layers"].GetArray();
		for (const auto& layer : layersArray) {
			if (layer.HasMember("numberOfCircles") && layer.HasMember("circlesColor") &&
				layer["numberOfCircles"].IsInt() && layer["circlesColor"].IsArray()) {
				auto color = layer["circlesColor"].GetArray();
				if (color.Size() == 3) {
					int numOfCircles = layer["numberOfCircles"].GetInt();
					int resultNumOfCircles = 0;
					if (numOfCircles <= 0)
						continue;
					else if (numOfCircles > 5)
						resultNumOfCircles = 5;
					else
						resultNumOfCircles = numOfCircles;
					layers_.push_back(
						std::make_pair(
							resultNumOfCircles,
							glm::fvec3(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat())
						)
					);
				}
			}
		}
	}

	if (layersSize != layers_.size()) {
		resourceManager_->clearObjects();
		createCircles();
		createLines();
		// Set camera to center of network
		engine_->setCameraPosition(glm::fvec2(50 * (layers_.size() - 1), 0.f));
		auto a = static_cast<float>((layers_.size() + 2) * 100) / static_cast<float>(WINDOW_SIZE.x);
		engine_->setCameraScale(
			static_cast<float>((layers_.size() + 2) * 100) / static_cast<float>(WINDOW_SIZE.x)
		);
	}
}
