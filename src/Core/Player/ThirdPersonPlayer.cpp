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
    
    if (Input()->ScrollUp())
        distance -= 1.f;
    if (Input()->ScrollDown())
        distance += 1.f;
    
    if (distance < 0.5f)
        distance = 0.5f;
    if (distance > 20.f)
        distance = 20.f;
    
    float rha = glm::radians(horizontalAngle);
    float rva = glm::radians(verticalAngle);
    
    glm::vec3 pos = follow->Position();
    pos += distance * glm::vec3(cos(rha) * cos(rva), sin(rva), sin(rha) * cos(rva));
    GetCamera()->SetPosition(pos);
    
    float hAngle = horizontalAngle + 270.f;
    GetCamera()->SetRotation(hAngle, verticalAngle, 0.f);
    
    if (GameSettings::GetInstance().GetBool("Center Mouse Cursor"))
		Input()->CenterCursor();
    
	prevMX = static_cast<float>(Input()->CursorX());
	prevMY = static_cast<float>(Input()->CursorY());
}
