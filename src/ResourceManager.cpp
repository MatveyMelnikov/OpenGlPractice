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

	/*if (source == "")
		return;


	rapidjson::Document document;
	rapidjson::ParseResult result = document.Parse(source.c_str());
	if (result.IsError()) {
		printf(
			"-Parse error in %s: %s (%u)\n", 
			filePath.c_str(), 
			rapidjson::GetParseError_En(result.Code()), 
			result.Offset()
		);
		return;
	}

	if (document.HasMember("shaders") && document["shaders"].IsObject()) {
		auto shaders = document["shaders"].GetObject();
		if (shaders.HasMember("vertex") && shaders.HasMember("fragment") &&
			shaders["vertex"].IsString() && shaders["fragment"].IsString()) {
			std::string vertexShader = shaders["vertex"].GetString();
			std::string fragmentShader = shaders["fragment"].GetString();
		} else {
			printf(
				"-Parse error: Ñan't find \"vertex\" and \"fragment\" members in %s",
				filePath.c_str()
			);
		}
	} else {
		printf(
			"-Parse error: Ñan't find \"shaders\" member in %s",
			filePath.c_str()
		);
	}*/
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
