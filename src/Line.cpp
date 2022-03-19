#include "Line.h"

Line::Line(
	glm::ivec2 startPoint,
	glm::ivec2 endPoint,
	unsigned int width,
	glm::fvec3 color,
	std::shared_ptr<ShaderProgram> shader
) : startPoint_(startPoint), endPoint_(endPoint), width_(width), color_(color), shader_(shader) {
	// Two triangles forming a rectangle.
	GLfloat points[] =
	{
		 0.0f,  0.0f,
		 0.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  0.0f,
		 0.0f,  0.0f,
	};
	// vbo
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(points),
		&points,
		GL_STATIC_DRAW
	);
	//vao
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	calculateMatrixes();
}

void Line::setPoints(glm::ivec2 startPoint, glm::ivec2 endPoint) {
	startPoint_ = startPoint;
	endPoint_ = endPoint;
	calculateMatrixes();
}

void Line::setColor(glm::fvec3 color) {
	color_ = color;
}

void Line::setWidth(unsigned int width) {
	width_ = width;
	calculateMatrixes();
}

std::shared_ptr<ShaderProgram> Line::getShader() {
	return shader_;
}

float Line::getAngleRelativeAxis(int deltaX, int deltaY) {
	const float PI = 3.1415;
	// alpha = |y1 - y2| / |x1 - x2|
	float angle = atan(abs((float)deltaY) / abs((float)deltaX));
	if (deltaX < 0 && deltaY >= 0) // 4 quarter
		return 2.0f * PI - angle; // (0.5f * PI - angle) + PI * 1.5f
	else if (deltaX > 0 && deltaY >= 0) // 3 quarter
		return angle + PI;
	else if (deltaX >= 0 && deltaY < 0) // 2 quarter
		return 1.0f * PI - angle; // (0.5f * PI - angle) + PI * 0.5f
	else
		return angle;
}

void Line::calculateMatrixes() {
	int deltaX = startPoint_.x - endPoint_.x;
	int deltaY = startPoint_.y - endPoint_.y;
	// alpha = |y1 - y2| / |x1 - x2|
	float length = sqrt(
		pow(deltaY, 2) +
		pow(deltaX, 2)
	);

	modelMatrix_ = glm::translate(
		glm::mat4(1.f),
		glm::vec3(
		((float)startPoint_.x),
			((float)startPoint_.y),
			0.f
		)
	);
	modelMatrix_ = glm::rotate(
		modelMatrix_,
		getAngleRelativeAxis(deltaX, deltaY),
		glm::vec3(0.f, 0.f, 1.f)
	);
	modelMatrix_ = glm::translate( // center offset
		modelMatrix_,
		glm::vec3(
			0.f,
			-(float)width_,
			0.f
		)
	);
	// x - length, y - width
	modelMatrix_ = glm::scale(
		modelMatrix_,
		glm::vec3(
			length,
			width_ * 2,
			1.f
		)
	);
}

void Line::render(const glm::ivec2& windowSize) {
	if (shader_ == nullptr)
		return;
	
	shader_->use();
	shader_->setUniform("color", color_);
	shader_->setUniform("modelMatrix", modelMatrix_);
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glUseProgram(0);
}
