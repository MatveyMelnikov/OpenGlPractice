#pragma once
#include <GLFW/glfw3.h>
#include <vector>
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
	void moveCamera(glm::fvec2 cameraPosition);
	void setCameraPosition(glm::fvec2 cameraPosition);
	void setCameraScale(float scale);
	float getCameraScale();
	void setCameraOffset(glm::fvec2 cameraOffset);
	float getLastFrameTime();
	void render(const glm::ivec2& windowSize);
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
	glm::fvec2 cameraPosition_ = glm::fvec2(0, 0);
	glm::fvec2 cameraOffset_ = glm::fvec2(0, 0);
	float scale_ = 1.0f;
	float lastFrameTime_ = 0.f;
};
