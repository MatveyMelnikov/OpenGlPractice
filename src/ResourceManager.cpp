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
void ResourceManager::createShaderProgram(const std::string& path) {
	std::string vertexShader = loadStringFromFile(path + "/vertexShader.txt");
	std::string fragmentShader = loadStringFromFile(path + "/fragmentShader.txt");
	if (!vertexShader.empty() && !fragmentShader.empty())
		createShaderProgram(vertexShader, fragmentShader);
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

void ResourceManager::createCircle(glm::ivec2 position, glm::fvec3 color, unsigned int shaderPosition) {
	createCircle(position, circlesRadius_, color, shaderPosition);
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

void ResourceManager::createLine(
	glm::ivec2 startPoint,
	glm::ivec2 endPoint,
	glm::fvec3 color,
	unsigned int shaderPosition
) {
	createLine(startPoint, endPoint, linesWidth_, color, shaderPosition);
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

std::string ResourceManager::loadStringFromFile(const std::string& path) {
	if (path.empty())
		return std::string();

	std::ifstream fin(path);
	if (!fin.is_open())
		return std::string();

	std::stringstream source;
	source << fin.rdbuf();
	fin.close();

	return source.str();
}

void ResourceManager::setDefaultParameters(float circlesRadius, unsigned int linesWidth) {
	circlesRadius_ = circlesRadius;
	linesWidth_ = linesWidth;
}

void ResourceManager::clearObjects() {
	circles_.clear();
	lines_.clear();
}

void ResourceManager::clearLines() {
	lines_.clear();
}

void ResourceManager::clearCirlces() {
	circles_.clear();
}
