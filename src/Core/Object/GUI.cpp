#include "GUI.hpp"
#include "../Resources.hpp"

GUI::GUI(const glm::vec2& screenSize) {
	barHeight = screenSize.y * 0.25f;
	barWidth = screenSize.x * 0.04f;
	barPosX = screenSize.x * 0.1f;
	barPosY = screenSize.y * 0.7f;
	barXOffset = screenSize.x * 0.05f;
    
    strikeBarTexture = Resources().CreateTexture2DFromFile("Resources/GUI/StrikeBar.png");
    strikeIndicatorTexture = Resources().CreateTexture2DFromFile("Resources/GUI/StrikeIndicator.png");
}

GUI::~GUI() {
    Resources().FreeTexture2DFromFile(strikeBarTexture);
    Resources().FreeTexture2DFromFile(strikeIndicatorTexture);
}

void GUI::Render(const glm::vec2& screenSize, std::vector<PlayerObject>& players, float swingStrength) {
	int index = 0;

	for (auto &player : players){
			// Background
			healthBar.Render(glm::vec2(barPosX + barXOffset*index, barPosY),
				glm::vec2(barWidth, barHeight),
				glm::vec3(0.1f, 0.1f, 0.1f),
				screenSize);

			if (player.getHealth() > 0){
				// Actual healthbar
				healthBar.Render(glm::vec2(barPosX + barXOffset*index, barPosY + (barHeight - barHeight*(player.getHealth() / 100.f))),
					glm::vec2(barWidth, barHeight*(player.getHealth() / 100.f)),
					glm::vec3(10.f, 0.f, 0.f),
					screenSize);
			}
			index++;
	}
    
	// Render power bar.
	glm::vec2 strikeBarSize(strikeBarTexture->Width() - (barPosX + barXOffset*index), strikeBarTexture->Height());
	strikeBarTexture->Render(glm::vec2(barPosX + barXOffset*index, barPosY - strikeBarSize.y + barHeight), strikeBarSize, screenSize);
    
    glm::vec2 strikeIndicatorSize(strikeIndicatorTexture->Width(), strikeIndicatorTexture->Height());
	strikeIndicatorTexture->Render(glm::vec2(barPosX + barXOffset*index + (strikeBarSize.x - strikeIndicatorSize.x * 0.5f) * swingStrength - strikeIndicatorSize.x * 0.5f, barPosY + barHeight - (strikeBarSize.y + strikeIndicatorSize.y) * 0.5f), strikeIndicatorSize, screenSize);
}
