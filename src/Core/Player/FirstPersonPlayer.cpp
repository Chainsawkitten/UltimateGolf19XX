#include "FirstPersonPlayer.hpp"
#include "../Camera/FirstPersonCamera.hpp"
#include "../Util/Input.hpp"
#include "../Util/GameSettings.hpp"

void FirstPersonPlayer::Update(double time) {
    float dx = static_cast<float>(time) * MovementSpeed() * (Input()->Pressed(InputHandler::RIGHT) - Input()->Pressed(InputHandler::LEFT));
	float dz = static_cast<float>(time) * MovementSpeed() * (Input()->Pressed(InputHandler::FORWARD) - Input()->Pressed(InputHandler::BACKWARD));

	glm::vec3 position = GetCamera()->Position();

	glm::vec3 forward = GetCamera()->Forward();
	forward.y = 0.f;
	forward = glm::normalize(forward);
	position += dz * forward;

	glm::vec3 right = GetCamera()->Right();
	right.y = 0.f;
	right = glm::normalize(right);
	position += dx * right;

	GetCamera()->SetPosition(position);

    float mdx = static_cast<float>(Input()->CursorX()) - prevMX;
	float mdy = static_cast<float>(Input()->CursorY()) - prevMY;
    float mouseSensitivity = static_cast<float>(GameSettings::GetInstance().GetDouble("Mouse Sensitivity"));
	GetCamera()->Rotate(mdx * 0.2f * mouseSensitivity, mdy * 0.2f * mouseSensitivity, 0.f);

	if (GameSettings::GetInstance().GetBool("Center Mouse Cursor"))
		Input()->CenterCursor();

	prevMX = static_cast<float>(Input()->CursorX());
	prevMY = static_cast<float>(Input()->CursorY());
}
