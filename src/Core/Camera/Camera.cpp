#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Camera::Camera() {
	fieldOfView = 45.f;
    zNear = 0.05f;
    zFar = 100.f;
}

glm::vec3 Camera::Forward() const {
	glm::vec4 forward = glm::inverse(Orientation()) * glm::vec4(0, 0, -1, 1);
	return glm::vec3(forward);
}
glm::vec3 Camera::Right() const {
	glm::vec4 right = glm::inverse(Orientation()) * glm::vec4(1, 0, 0, 1);
	return glm::vec3(right);
}
glm::vec3 Camera::Up() const {
	glm::vec4 up = glm::inverse(Orientation()) * glm::vec4(0, 1, 0, 1);
	return glm::vec3(up);
}

glm::mat4 Camera::View() const {
	return Orientation() * glm::translate(glm::mat4(), -Position());
}

float Camera::FieldOfView() const {
	return fieldOfView;
}

void Camera::SetFieldOfView(float fieldOfView) {
	this->fieldOfView = fieldOfView;
}

float Camera::NearPlane() const {
	return zNear;
}

float Camera::FarPlane() const {
	return zFar;
}

void Camera::SetNearAndFarPlanes(float near, float far) {
	zNear = near;
	zFar = far;
}

glm::mat4 Camera::Projection(const glm::vec2& screenSize) const {
	return glm::perspective(glm::radians(fieldOfView), screenSize.x / screenSize.y, zNear, zFar);
}
