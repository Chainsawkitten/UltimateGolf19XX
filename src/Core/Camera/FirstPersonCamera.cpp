#include "FirstPersonCamera.hpp"

void FirstPersonCamera::Move(const glm::vec3& offset) {
	glm::vec3 movement;
	movement += offset.x * Right();
	movement += offset.y * Up();
	movement += offset.z * Forward();

	Camera::Move(movement);
}

void FirstPersonCamera::Rotate(float horizontalAngle, float verticalAngle, float tiltAngle) {
	horizontalAngle = fmodf(Camera::HorizontalAngle() + horizontalAngle, 360.f);
	if (horizontalAngle < 0.f)
		horizontalAngle += 360.f;

	verticalAngle = Camera::VerticalAngle() + verticalAngle;
	if (verticalAngle > 85.f)
		verticalAngle = 85.f;
	else if (verticalAngle < -85.f)
		verticalAngle = -85.f;

	tiltAngle = fmodf(Camera::TiltAngle() + tiltAngle, 360.f);
	if (tiltAngle < 0.f)
		tiltAngle += 360.f;

	Camera::SetRotation(horizontalAngle, verticalAngle, tiltAngle);
}
