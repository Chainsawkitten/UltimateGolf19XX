#include "Object.hpp"
#include <glm/gtc/matrix_transform.hpp>

Object::Object() {
    position = glm::vec3(0.f, 0.f, 0.f);
    scale = glm::vec3(1.f, 1.f, 1.f);
    
    horizontalAngle = 0.f;
    verticalAngle = 0.f;
    tiltAngle = 0.f;
}

const glm::vec3& Object::Position() const {
	return position;
}

void Object::SetPosition(const glm::vec3& position) {
	this->position = position;
}

void Object::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

const glm::vec3& Object::Scale() const {
	return scale;
}

void Object::SetScale(const glm::vec3& scale) {
	this->scale = scale;
}

void Object::SetScale(float xScale, float yScale, float zScale) {
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;
}

void Object::Move(const glm::vec3& offset) {
	position += offset;
}

float Object::HorizontalAngle() const {
	return horizontalAngle;
}

float Object::VerticalAngle() const {
	return verticalAngle;
}

float Object::TiltAngle() const {
	return tiltAngle;
}

void Object::SetRotation(float horizontalAngle, float verticalAngle, float tiltAngle) {
	this->horizontalAngle = horizontalAngle;
	this->verticalAngle = verticalAngle;
	this->tiltAngle = tiltAngle;
}

void Object::Rotate(float horizontalAngle, float verticalAngle, float tiltAngle) {
	this->horizontalAngle = fmodf(horizontalAngle + this->horizontalAngle, 360.f);
	if (this->horizontalAngle < 0.f)
		this->horizontalAngle += 360.f;

	this->verticalAngle = fmodf(verticalAngle + this->verticalAngle, 360.f);
	if (this->verticalAngle < 0.f)
		this->verticalAngle += 360.f;

	this->tiltAngle = fmodf(tiltAngle + this->tiltAngle, 360.f);
	if (this->tiltAngle < 0.f)
		this->tiltAngle += 360.f;
}

glm::mat4 Object::Orientation() const {
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, glm::radians(tiltAngle), glm::vec3(0, 0, 1));
	orientation = glm::rotate(orientation, glm::radians(verticalAngle), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(horizontalAngle), glm::vec3(0, 1, 0));
	return orientation;
}

glm::mat4 Object::ModelMatrix() const {
	glm::mat4 model;
	model = glm::translate(glm::mat4(), position) * Orientation() * glm::scale(glm::mat4(), scale);
	return model;
}
