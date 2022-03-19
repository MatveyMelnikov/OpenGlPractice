#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "Circle.h"
#include "Line.h"

class ResourceManager {
public:
	static ResourceManager* getInstance();
	void createShaderProgram(const std::string& vertexShader, const std::string fragmentShader);
	void createShaderProgram(const std::string& file);
	std::shared_ptr<ShaderProgram> getShaderProgram(unsigned int position);
	unsigned int getShaderProgramsAmount();
	void createCircle(glm::ivec2 position, float radius, glm::fvec3 color, unsigned int shaderPosition);
	std::shared_ptr<Circle> getCircle(unsigned int position);
	unsigned int getCirclesAmount();
	void createLine(
		glm::ivec2 startPoint,
		glm::ivec2 endPoint,
		unsigned int width,
		glm::fvec3 color,
		unsigned int shaderPosition
	);
	std::shared_ptr<Line> getLine(unsigned int position);
	unsigned int getLinesAmount();
private:
	ResourceManager() {};
	ResourceManager(const ResourceManager&) {};
	ResourceManager& operator=(ResourceManager&) = delete;
	void catchMovementKeys();
private:
	static ResourceManager* instance_;
	std::vector<std::shared_ptr<ShaderProgram>> shaderPrograms_;
	std::vector<std::shared_ptr<Circle>> circles_;
	std::vector<std::shared_ptr<Line>> lines_;
};
