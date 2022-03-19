#include "Network.h"
Network* Network::instance_ = nullptr;

Network* Network::getInstance(GLFWAPI::GLFWwindow* window, const std::string& networkFilePath) {
	if (instance_ == nullptr)
		instance_ = new Network(window, networkFilePath);
	return instance_;
}

Network::Network(GLFWAPI::GLFWwindow* window, const std::string& networkFilePath) {
	engine_ = Engine::getInstance(window);
	resourceManager_ = ResourceManager::getInstance();
	networkFilePath_ = networkFilePath;
	updateNetwork();
}

void Network::updateNetwork() {
	if (engine_ == nullptr)
		return;
	layers_.clear();
	resourceManager_->clearObjects();

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
					layers_.push_back(
						std::make_pair(
							layer["numberOfCircles"].GetInt(),
							glm::fvec3(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat())
						)
					);
				}
			}
		}
	}

	createCircles();
	createLines();

	// Set camera to center of network
	engine_->setCameraPosition(glm::fvec2(50 * (layers_.size() - 1), 0.f));
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
				10,
				layers_[i].second,
				0
			);
		}
	}
}

void Network::createLines() {
	unsigned int offset = 0; // Before startPoint
	for (unsigned int i = 0; i < layers_.size() - 1; i++) {
		for (unsigned int j = 0; j < layers_[i].first; j++) { // startPoint
			for (unsigned int k = 0; k < layers_[i + 1].first; k++) { // endPoint
				resourceManager_->createLine(
					resourceManager_->getCircle(j + offset)->getPosition(),
					resourceManager_->getCircle(k + offset + layers_[i].first)->getPosition(),
					5,
					glm::fvec3(0.3f, 0.3f, 0.3f) + glm::fvec3(0.1f * j, 0.1f * j, 0.1f * j),
					0
				);
			}
		}
		offset += layers_[i].first;
	}
}
