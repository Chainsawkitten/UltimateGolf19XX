#include "GUI.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include <glm/glm.hpp>

GUI::GUI(const glm::vec2& screenSize){
	barHeight = screenSize.y*0.25f;
	barWidth = screenSize.x*0.04f;
	barPosX = screenSize.x*0.1f;
	barPosY = screenSize.y*0.7f;
	barXOffset = screenSize.x*0.05f;
}

GUI::~GUI(){
}

void GUI::Render(const glm::vec2& screenSize, std::vector<PlayerObject>& players, float swingStrength){
	int index = 0;

	for (auto &player : players){
		if (player.getHealth() > 0){
			//Background
			healthBar.Render(glm::vec2(barPosX + barXOffset*index, barPosY),
				glm::vec2(barWidth, barHeight),
				glm::vec3(0.1f, 0.1f, 0.1f),
				screenSize);

			//Actual healthbar
			healthBar.Render(glm::vec2(barPosX+barXOffset*index, barPosY + (barHeight - barHeight*(player.getHealth()/100.f))),
				glm::vec2(barWidth, barHeight*(player.getHealth()/100.f)),
				glm::vec3(10.f, 0.f, 0.f),
				screenSize);
			index++;
		}
	}
	//Powerbar background
	powerBar.Render(glm::vec2(barPosX + barXOffset*index, barPosY + barHeight - barWidth),
		glm::vec2(barHeight, barWidth),
		glm::vec3(0.1f, 0.1f, 0.1f),
		screenSize);

	//Powerbar
	powerBar.Render(glm::vec2(barPosX + barXOffset*index, barPosY + barHeight - barWidth),
		glm::vec2(barHeight*(swingStrength/100.f), barWidth),
		glm::vec3(0.f, 10.f, 0.f),
		screenSize);
}