#pragma once
#include <glad/glad.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Circle {
public:
	Circle(glm::ivec2 position, unsigned int radius, glm::fvec3 color, std::shared_ptr<ShaderProgram> shader);
	void setPosition(glm::vec2 position);
	void setColor(glm::fvec3 color);
	void setPrecision(unsigned int precision);
	void setRadius(unsigned int radius);
	glm::ivec2 getPosition();
	std::shared_ptr<ShaderProgram> getShader();
	void render(const glm::ivec2& windowSize);
private:
	void updatePrecision();
private:
	std::vector<glm::fvec2> points_;
	glm::ivec2 position_;
	unsigned int radius_;
	glm::fvec3 color_;
	GLuint vbo_ = 0;
	GLuint vao_ = 0;
	std::shared_ptr<ShaderProgram> shader_ = nullptr;
	unsigned int precision_ = 20;
};
