#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string fragmentShader) {
	GLuint vertexShaderId = 0, segmentShaderId = 0;

	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexSource = vertexShader.c_str();
	glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
	glCompileShader(vertexShaderId);

	segmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentSource = fragmentShader.c_str();
	glShaderSource(segmentShaderId, 1, &fragmentSource, nullptr);
	glCompileShader(segmentShaderId);

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShaderId);
	glAttachShader(shaderProgram_, segmentShaderId);
	glLinkProgram(shaderProgram_);

	glDeleteShader(vertexShaderId);
	glDeleteShader(segmentShaderId);
}

void ShaderProgram::use() {
	glUseProgram(shaderProgram_);
}

void ShaderProgram::setUniform(const std::string& uniformName, GLint value) {
	glUniform1i(glGetUniformLocation(shaderProgram_, uniformName.c_str()), value);
}

void ShaderProgram ::setUniform(const std::string& uniformName, glm::fvec3 value) {
	glUniform3f(glGetUniformLocation(shaderProgram_, uniformName.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string& uniformName, glm::ivec3 value) {
	glUniform3i(glGetUniformLocation(shaderProgram_, uniformName.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string& uniformName, glm::ivec4 value) {
	glUniform4i(glGetUniformLocation(shaderProgram_, uniformName.c_str()), value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::mat4& value) {
	glUniformMatrix4fv(
		glGetUniformLocation(shaderProgram_, uniformName.c_str()),
		1, 
		false, 
		&value[0][0]
	);
}
