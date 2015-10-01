#pragma once

#include "GeometryObject.hpp"
#include "../Shader/ShaderProgram.hpp"

/** @ingroup Core
 * @{
 */

/// Reflective water surface.
class Water : public GeometryObject {
    public:
        /// Create new water.
        Water();
        
        /// Destructor.
        ~Water();
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
    private:
        // Water shaders.
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
};

/** @} */
