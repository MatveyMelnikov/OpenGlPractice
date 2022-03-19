#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string fragmentShader) {
	GLuint vertexShaderId = 0, fragmentShaderId = 0;

	const GLchar* vertexSource = vertexShader.c_str();
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
	glCompileShader(vertexShaderId);
	debugShaderCompile(vertexShaderId);

	const GLchar* fragmentSource = fragmentShader.c_str();
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShaderId);
	debugShaderCompile(fragmentShaderId);

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShaderId);
	glAttachShader(shaderProgram_, fragmentShaderId);
	glLinkProgram(shaderProgram_);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

void ShaderProgram::use() {
	glUseProgram(shaderProgram_);
}

void ShaderProgram::setUniform(const std::string& uniformName, GLint value) {
	glUniform1i(glGetUniformLocation(shaderProgram_, uniformName.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& uniformName, glm::fvec3 value) {
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

void ShaderProgram::debugShaderCompile(GLuint shaderId) {
	GLint is_compile = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &is_compile);
	if (!is_compile)
	{
		GLchar info_log[1024];
		glGetShaderInfoLog(shaderId, 1024, nullptr, info_log);
		printf("-Shader compile error: %s\n", info_log);
	}
}
