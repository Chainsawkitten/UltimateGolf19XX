#pragma once

#include "Skybox.hpp"
#include "Player/Player.hpp"
#include "Util/Input.hpp"
#include "RenderTarget.hpp"
#include "Object/GeometryObject.hpp"
#include "Texture/Texture2D.hpp"
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
        CubeMapTexture* skyboxTexture;
        Skybox* skybox;
        Player* player;
        RenderTarget* renderTarget;
        Geometry::Geometry3D* geometry;
        GeometryObject* geometryObject;
        Texture2D* texture;
        
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        // Input
        InputHandler* input;
};

/** @} */
