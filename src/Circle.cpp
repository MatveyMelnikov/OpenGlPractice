#include "Circle.h"

Circle::Circle(
	glm::ivec2 position,
	unsigned int radius,
	glm::fvec3 color,
	std::shared_ptr<ShaderProgram> shader
) : position_(position), radius_(radius), color_(color), shader_(shader) {
	updatePrecision();
	// vbo
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(points_[0].x) * 2 * points_.size(),
		&points_[0].x,
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

// Sets the position of the circle in pixels.
void Circle::setPosition(glm::vec2 position) {
	position_ = position;
	calculateMatrixes();
}

void Circle::setColor(glm::fvec3 color) {
	color_ = color;
}

// Sets the number of points that will define the circle. 3 - minimum.
void Circle::setPrecision(unsigned int precision) {
	if (precision >= 3)
		precision_ = precision;
	updatePrecision();
}

void Circle::setRadius(unsigned int radius) {
	radius_ = radius;
	calculateMatrixes();
}

unsigned int Circle::getRadius() {
	return radius_;
}

glm::ivec2 Circle::getPosition() {
	return position_;
}

std::shared_ptr<ShaderProgram> Circle::getShader() {
	return shader_;
}

void Circle::render(const glm::ivec2& windowSize) {
	if (shader_ == nullptr)
		return;

	shader_->use();
	shader_->setUniform("color", color_);
	shader_->setUniform("modelMatrix", modelMatrix_);
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLE_FAN, 0, points_.size());
}

void Circle::updatePrecision() {
	const float PI = 3.1415;
	points_.clear();
	points_.resize(precision_ + 1);
	float cornerPart = (2 * PI) / precision_;
	for (unsigned int i = 0; i < precision_; i++) { // x = cos(alpha) * r, y = sin(alpha) * r, r = 1
		points_[i] = glm::fvec2(
			cos(cornerPart * i),
			sin(cornerPart * i)
		);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(points_[0].x) * 2 * points_.size(),
		&points_[0].x,
		GL_STATIC_DRAW
	);
	glUseProgram(0);
}

void Circle::calculateMatrixes() {
	modelMatrix_ = glm::translate(
		glm::mat4(1.f),
		glm::vec3(
			position_.x,
			position_.y,
			0.f
		)
	);
	modelMatrix_ = glm::scale(
		modelMatrix_,
		glm::vec3(
			radius_ * 2.f,
			radius_ * 2.f,
			1.f
		)
	);
}