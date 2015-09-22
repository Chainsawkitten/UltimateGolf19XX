#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

/// The player character.
/**
 * Holds a Camera object and manipulates that camera's position and rotation based on player input (free floating first person camera).
 */
class Player {
	public:
		/// Creates a new player object.
		Player();

		/// Free allocated resources.
		virtual ~Player();

		/// Get movement speed.
		/**
		 * @return Movement speed (units/second)
		 */
		float MovementSpeed() const;

		/// Set movement speed.
		/**
		 * @param movementSpeed Movement speed (units/second).
		 */
		void SetMovementSpeed(float movementSpeed);

		/// Update movement.
		/**
		 * Updates position and orientation based on player input.
		 * @param time Time since last frame (in seconds).
		 */
		virtual void Update(double time);

		/// Get player camera.
		/**
		 * @return The player's Camera object
		 */
		Camera* GetCamera() const;

	protected:
		float prevMX, prevMY;

	private:
		Camera* camera;
		float movementSpeed;
};

/** @} */

#endif
