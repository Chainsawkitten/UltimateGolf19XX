#include "GUI.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include <glm/glm.hpp>

GUI::GUI(int players){
	healthBar = new Geometry::Rectangle[players];
}

GUI::~GUI(){
	delete healthBar;
}

void GUI::Render(const glm::vec2& screenSize, std::vector<PlayerObject>& players, float swingStrength){
	int index = 0;
	for (auto &player : players){
		if (player.getHealth() > 0){
			healthBar[index].Render(glm::vec2(screenSize.x*(0.1f + (index*0.05)), screenSize.y*0.7f + (100.f*screenSize.y*0.0025f - screenSize.y*0.0025f*player.getHealth())),
				glm::vec2(screenSize.x*0.04f, screenSize.y*0.0025f*player.getHealth()),
				glm::vec3(10.f, 0.f, 0.f),
				screenSize);
			index++;
		}
	}
	powerBar.Render(glm::vec2(screenSize.x*(0.1f + (index*0.05)), screenSize.y*0.7f + (100.f*screenSize.y*0.0025f) - screenSize.y*0.04f),
		glm::vec2(swingStrength, screenSize.y*0.04f),
		glm::vec3(0.f, 10.f, 0.f),
		screenSize);
}