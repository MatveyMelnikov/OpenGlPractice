#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

ResourceManager* ResourceManager::getInstance() {
	if (instance_ == nullptr)
		instance_ = new ResourceManager;
	return instance_;
}

void ResourceManager::createShaderProgram(const std::string& vertexShader, const std::string fragmentShader) {
	shaderPrograms_.push_back(
		std::make_shared<ShaderProgram>(vertexShader, fragmentShader)
	);
}
void ResourceManager::createShaderProgram(const std::string& file) {
	// TODO
}

std::shared_ptr<ShaderProgram> ResourceManager::getShaderProgram(unsigned int position) {
	if (position >= shaderPrograms_.size())
		return nullptr;
	else
		return shaderPrograms_[position];
}

unsigned int ResourceManager::getShaderProgramsAmount() {
	return shaderPrograms_.size();
}

void ResourceManager::createCircle(glm::ivec2 position, float radius, glm::fvec3 color, unsigned int shaderPosition) {
	std::shared_ptr<ShaderProgram> shader = getShaderProgram(shaderPosition);
	if (shader == nullptr)
		return;

	circles_.push_back(
		std::make_shared<Circle>(position, radius, color, shader)
	);
}

std::shared_ptr<Circle> ResourceManager::getCircle(unsigned int position) {
	if (position >= circles_.size())
		return nullptr;
	else
		return circles_[position];
}

unsigned int ResourceManager::getCirclesAmount() {
	return circles_.size();
}

void ResourceManager::createLine(
	glm::ivec2 startPoint,
	glm::ivec2 endPoint,
	unsigned int width,
	glm::fvec3 color,
	unsigned int shaderPosition
) {
	std::shared_ptr<ShaderProgram> shader = getShaderProgram(shaderPosition);
	if (shader == nullptr)
		return;

	lines_.push_back(std::make_shared<Line>(startPoint, endPoint, width, color, shader));
}

std::shared_ptr<Line> ResourceManager::getLine(unsigned int position) {
	if (position >= lines_.size())
		return nullptr;
	else
		return lines_[position];
}

unsigned int ResourceManager::getLinesAmount() {
	return lines_.size();
}

void ResourceManager::moveCamera(glm::fvec2 cameraPosition) {
	cameraPosition_ += cameraPosition;
}

void ResourceManager::setCameraPosition(glm::fvec2 cameraPosition) {
	cameraPosition_ = glm::fvec2(
		cameraPosition.x,
		cameraPosition.y
	);
}

void ResourceManager::setCameraScale(float scale) {
	scale_ = scale;
}

float ResourceManager::getCameraScale() {
	return scale_;
}

void ResourceManager::setCameraOffset(glm::fvec2 cameraOffset) {
	cameraOffset_ = cameraOffset;
}

float ResourceManager::getLastFrameTime() {
	return lastFrameTime_;
}

void ResourceManager::render(const glm::ivec2& windowSize) {
	glm::mat4 projectionMatrix = glm::ortho(0.f, (float)windowSize.x, 0.f, (float)windowSize.y, 0.f, 10.f);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(
			cameraPosition_.x - cameraOffset_.x,
			cameraPosition_.y - cameraOffset_.y,
			1.f
		),
		glm::vec3(
			cameraPosition_.x - cameraOffset_.x,
			cameraPosition_.y - cameraOffset_.y, 
			0.f
		), 
		glm::vec3(0.f, 1.f, 0.f)
	);

	for (auto i : lines_) {
		i->getShader()->setUniform("viewMatrix", projectionMatrix * viewMatrix);
		i->render(windowSize);
	}
	for (auto i : circles_) {
		i->getShader()->setUniform("viewMatrix", projectionMatrix * viewMatrix);
		i->render(windowSize);
	}

	lastFrameTime_ = glfwGetTime();
}
