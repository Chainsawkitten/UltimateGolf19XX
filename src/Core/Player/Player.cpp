#include "Player.hpp"
#include "../Camera/FirstPersonCamera.hpp"
#include "../Util/Input.hpp"
#include "../Util/GameSettings.hpp"

Player::Player() {
	camera = new FirstPersonCamera();
	camera->SetPosition(glm::vec3(0.f, 0.f, 2.f));
	
	Input()->CenterCursor();
	prevMX = static_cast<float>(Input()->CursorX());
	prevMY = static_cast<float>(Input()->CursorY());
    
    movementSpeed = 1.f;
}

Player::~Player() {
	delete camera;
}

float Player::MovementSpeed() const {
	return movementSpeed;
}

void Player::SetMovementSpeed(float movementSpeed) {
	this->movementSpeed = movementSpeed;
}

void Player::Update(double time) {
	float dx = static_cast<float>(time) * movementSpeed * (Input()->Pressed(InputHandler::RIGHT) - Input()->Pressed(InputHandler::LEFT));
	float dz = static_cast<float>(time) * movementSpeed * (Input()->Pressed(InputHandler::FORWARD) - Input()->Pressed(InputHandler::BACKWARD));
	camera->Move(glm::vec3(dx, 0.f, dz));
    
	float mdx = static_cast<float>(Input()->CursorX()) - prevMX;
	float mdy = static_cast<float>(Input()->CursorY()) - prevMY;
    float mouseSensitivity = static_cast<float>(GameSettings::GetInstance().GetDouble("Mouse Sensitivity"));
	camera->Rotate(mdx * 0.2f * mouseSensitivity, mdy * 0.2f * mouseSensitivity, 0.f);
    
    if (GameSettings::GetInstance().GetBool("Center Mouse Cursor"))
		Input()->CenterCursor();
    
	prevMX = static_cast<float>(Input()->CursorX());
	prevMY = static_cast<float>(Input()->CursorY());
}

Camera* Player::GetCamera() const {
	return camera;
}
