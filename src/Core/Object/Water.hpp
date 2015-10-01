#pragma once

#include "GeometryObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../RenderTarget.hpp"

/** @ingroup Core
 * @{
 */

/// Reflective water surface.
class Water : public GeometryObject {
    public:
        /// Create new water.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        Water(const glm::vec2& screenSize);
        
        /// Destructor.
        ~Water();
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
        /// Get refraction render target.
        /**
         * @return The refraction render target
         */
        RenderTarget* RefractionTarget() const;
        
        /// Get reflection render target.
        /**
         * @return The reflection render target
         */
        RenderTarget* ReflectionTarget() const;
        
    private:
        // Water shaders.
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        // Render targets.
        RenderTarget* refractionTarget;
        RenderTarget* reflectionTarget;
        
};

/** @} */
