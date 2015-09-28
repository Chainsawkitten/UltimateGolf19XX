#pragma once

#include "GeometryObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"

/** @ingroup Core
 * @{
 */

/// A golf ball that can be struck.
class GolfBall : public GeometryObject {
    public:
        /// Create new golf ball.
        GolfBall();
        
        /// Destructor.
        ~GolfBall();
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
    private:
        Texture2D* texture;
        
        // Shaders
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
};

/** @} */