#ifndef FIRSTPERSONPLAYER_HPP
#define FIRSTPERSONPLAYER_HPP

#include "Player.hpp"

/** @ingroup Core
 * @{
 */

/// The player character.
/**
 * Holds a Camera object and manipulates that camera's position and rotation based on player input (grounded first person camera).
 */
class FirstPersonPlayer : public Player {
	public:
		/// Update movement.
		/**
		 * Updates position and orientation based on player input.
		 * @param time Time since last frame (in seconds).
		 */
		void Update(double time);
};

/** @} */

#endif
