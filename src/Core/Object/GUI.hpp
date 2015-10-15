#pragma once

#include "GeometryObject.hpp"
#include "../Object/PlayerObject.hpp"
#include "../Geometry/Rectangle.hpp"
#include "../Texture/Texture2D.hpp"

/** @ingroup Core
* @{
*/

/// Simply GUI with healthbars and powerbar.
class GUI {
public:
	///Create new GUI.
	/**
	 * @param screenSize vec2 containing size of screen in pixels.
	 */
	GUI(const glm::vec2& screenSize);

	///Destroy GUI.
	~GUI();

	///Render the GUI.
	/**
	 * @param screenSize vec2 containing size of screen in pixels.
	 * @param players Vector containing PlayerObjects.
	 * @param swingStrength strength of swing 0-100.
	 */
	void Render(const glm::vec2& screenSize, std::vector<PlayerObject>& players, float swingStrength);

private:
	Geometry::Rectangle healthBar;
	Geometry::Rectangle powerBar;
    
    Texture2D* strikeBarTexture;
    Texture2D* strikeIndicatorTexture;

	float barHeight;
	float barWidth;
	float barPosX;
	float barPosY;
	float barXOffset;
};

/** @} */
