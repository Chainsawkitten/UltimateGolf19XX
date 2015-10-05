#include "ThirdPersonPlayer.hpp"
#include "../Util/GameSettings.hpp"
#include "../Util/Input.hpp"
#include <cmath>
#include <glm/gtc/constants.hpp>

ThirdPersonPlayer::ThirdPersonPlayer(Object *follow) {
    this->follow = follow;
    distance = 10.f;
    horizontalAngle = 0.f;
    verticalAngle = 0.f;
}

void ThirdPersonPlayer::Update(double time) {
    float mdx = static_cast<float>(Input()->CursorX()) - prevMX;
	float mdy = static_cast<float>(Input()->CursorY()) - prevMY;
    float mouseSensitivity = static_cast<float>(GameSettings::GetInstance().GetDouble("Mouse Sensitivity"));
    horizontalAngle += mdx * 0.2f * mouseSensitivity;
    verticalAngle += mdy * 0.2f * mouseSensitivity;
    
    glm::vec3 pos = follow->Position();
    pos += distance * glm::vec3(cos(glm::radians(horizontalAngle)), 0.f, sin(glm::radians(horizontalAngle)));
    GetCamera()->SetPosition(pos);
    
    float hAngle = horizontalAngle + 270.f;
    GetCamera()->SetRotation(hAngle, 0.f, 0.f);
    
    if (GameSettings::GetInstance().GetBool("Center Mouse Cursor"))
		Input()->CenterCursor();
    
	prevMX = static_cast<float>(Input()->CursorX());
	prevMY = static_cast<float>(Input()->CursorY());
}
