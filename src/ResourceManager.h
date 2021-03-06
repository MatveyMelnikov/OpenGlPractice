#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "Circle.h"
#include "Line.h"

const std::string RESOURCES_PATH = "../res/";

class ResourceManager {
public:
	static ResourceManager* getInstance();
	void createShaderProgram(const std::string& vertexShader, const std::string fragmentShader);
	void createShaderProgram(const std::string& path);
	std::shared_ptr<ShaderProgram> getShaderProgram(unsigned int position);
	unsigned int getShaderProgramsAmount();
	void createCircle(glm::ivec2 position, float radius, glm::fvec3 color, unsigned int shaderPosition);
	void createCircle(glm::ivec2 position, glm::fvec3 color, unsigned int shaderPosition);
	std::shared_ptr<Circle> getCircle(unsigned int position);
	unsigned int getCirclesAmount();
	void createLine(
		glm::ivec2 startPoint,
		glm::ivec2 endPoint,
		unsigned int width,
		glm::fvec3 color,
		unsigned int shaderPosition
	);
	void createLine(
		glm::ivec2 startPoint,
		glm::ivec2 endPoint,
		glm::fvec3 color,
		unsigned int shaderPosition
	);
	std::shared_ptr<Line> getLine(unsigned int position);
	unsigned int getLinesAmount();
	std::string loadStringFromFile(const std::string& path);
	void setDefaultParameters(float circlesRadius, unsigned int linesWidth);
	void clearObjects();
	void clearLines();
	void clearCirlces();
private:
	ResourceManager() {};
	ResourceManager(const ResourceManager&) {};
	ResourceManager& operator=(ResourceManager&) = delete;
private:
	static ResourceManager* instance_;
	std::vector<std::shared_ptr<ShaderProgram>> shaderPrograms_;
	std::vector<std::shared_ptr<Circle>> circles_;
	std::vector<std::shared_ptr<Line>> lines_;
	float circlesRadius_ = 10;
	unsigned int linesWidth_ = 3;
};
