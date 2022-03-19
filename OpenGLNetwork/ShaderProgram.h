#pragma once
#include <string>
#include <glad/glad.h>
#include "glm.hpp"
#include "mat4x4.hpp"

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexShader, const std::string fragmentShader);
	void use();
	void setUniform(const std::string& uniformName, int value);
	void setUniform(const std::string& uniformName, glm::fvec3 value);
	void setUniform(const std::string& uniformName, glm::ivec3 value);
	void setUniform(const std::string& uniformName, glm::ivec4 value);
	void setUniform(const std::string& uniformName, const glm::mat4& value);
private:
	GLuint shaderProgram_ = 0;
};