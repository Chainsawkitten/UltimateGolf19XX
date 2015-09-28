#pragma once

#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

/// Interface for scenes in the game, eg. title screen or in-game.
class Scene {
	public:
		/// Returned when the scene has ended. Defines what should be done next.
		/**
		* Sample usage:
		* \code{.cpp}
		* SceneEnd* ExampleScene::Update(double time) {
		*     if (Input().Triggered(InputHandler::FORWARD))
		*         return new SceneEnd(SceneEnd::NEW_SCENE);
		*     return nullptr;
		* }
		* \endcode
		*/
		struct SceneEnd {
			public:
				/// Available commands.
				enum Command {
					/// Quit the game.
					QUIT = 0,
					/// Go to a new scene.
					NEW_SCENE
				};

				/// What to do next.
				Command command;

				/// Create a new SceneEnd and define what should be done next.
				/**
				 * @param command What to do.
				 */
				SceneEnd(Command command);

			private:
				SceneEnd();
		};

		/// Destructor
		virtual ~Scene() { }

		/// Update the scene.
		/**
		 * @param time Time since last frame (in seconds).
		 * @return SceneEnd-struct defining what to do next. nullptr if nothing should be done
		 */
		virtual SceneEnd* Update(double time) = 0;

		/// Render the scene.
		/**
		 * @param screenSize Size of the screen in pixels.
		 */
		virtual void Render(const glm::vec2& screenSize) = 0;
};

/** @} */
