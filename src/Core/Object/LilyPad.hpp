#pragma once

#include "Object.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Geometry/Square.hpp"
#include "../Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

/// A bundle of lilypad leaves.
class LilyPad : public Object {
    public:
        /// Create new duck.
        LilyPad();
        
        /// Destructor.
        ~LilyPad();
        
        /// Render the object.
        /**
         * @param shaderProgram %Shader program to render the object with.
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         * @param clippingPlane Clipping plane equation.
         */
        void Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const;
        
    private:
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        Geometry::Square* geometry;
        Texture2D* texture;
};

/** @} */
