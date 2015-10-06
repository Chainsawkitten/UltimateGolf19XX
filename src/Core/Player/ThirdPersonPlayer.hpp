#pragma once

#include "Player.hpp"

/** @ingroup Core
 * @{
 */

/// Third person player.
/**
 * Holds a Camera object and manipulates that camera's position and rotation based on player input (around an object).
 */
class ThirdPersonPlayer : public Player {
    public:
        /// Create new third person player.
        /**
         * @param follow Object to follow.
         */
        ThirdPersonPlayer(Object* follow);
        
        /// Update movement.
		/**
		 * Updates position and orientation based on player input.
		 * @param time Time since last frame (in seconds).
		 */
		void Update(double time);
        
    private:
        Object* follow;
        
        float distance;
        float desiredDistance;
        float horizontalAngle;
        float verticalAngle;
};

/** @} */
