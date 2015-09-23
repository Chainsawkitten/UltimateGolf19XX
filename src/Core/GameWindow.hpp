#pragma once

#include "Util/Input.hpp"
#include "Scene/Scene.hpp"
#include <GLFW/glfw3.h>

/** @ingroup Core
 * @{
 */

/// Handles the game window.
class GameWindow {
    public:
        /// Create new game window.
        GameWindow();

        /// Destructor.
        ~GameWindow();
        
        /// Initialize components.
        void Init();

        /// Get whether the game window should close.
        /**
         * @return Whether the game window should close
         */
        bool ShouldClose() const;

        /// Update the game.
        void Update();

        /// Render the game.
        void Render();
        
        /// Render the game.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        void Render(const glm::vec2& screenSize);

    private:
        void AssignKeyboardBindings();
        
        GLFWwindow* window;
        Scene* currentScene;
        
        double lastTime, lastTimeRender;
        
        glm::vec2 size;
        
        // Input
        InputHandler* input;
};

/** @} */
