#pragma once

#include "GeometryObject.hpp"
#include "../Geometry/Rectangle.hpp"
#include "../Object/PlayerObject.hpp"

/** @ingroup Core
* @{
*/

/// Simply GUI with healthbars and powerbar
class GUI {
public:
	///Create new GUI
	/**
	* @param players, number of players
	*/
	GUI(int players);

	///Destroy GUI
	~GUI();

	///Render the GUI
	void Render(const glm::vec2& screenSize, std::vector<PlayerObject>& players, float swingStrength);

private:
	Geometry::Rectangle* healthBar;
	Geometry::Rectangle powerBar;
};

/** @} */
