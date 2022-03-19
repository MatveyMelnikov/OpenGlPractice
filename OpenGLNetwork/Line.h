#pragma once
#include <glad/glad.h>
#include <vector>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Line {
public:
	Line(
		glm::ivec2 startPoint,
		glm::ivec2 endPoint,
		unsigned int width,
		glm::fvec3 color,
		std::shared_ptr<ShaderProgram> shader
	);
	void setPoints(glm::ivec2 startPoint, glm::ivec2 endPoint);
	void setColor(glm::fvec3 color);
	void setWidth(unsigned int width);
	std::shared_ptr<ShaderProgram> getShader();
	void render(const glm::ivec2& windowSize);
private:
	float getAngleRelativeAxis(int deltaX, int deltaY);
private:
	glm::ivec2 startPoint_;
	glm::ivec2 endPoint_;
	glm::fvec3 color_;
	GLuint vbo_ = 0;
	GLuint vao_ = 0;
	std::shared_ptr<ShaderProgram> shader_ = nullptr;
	unsigned int width_ = 5;
};