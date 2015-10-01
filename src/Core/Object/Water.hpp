#pragma once

#include "GeometryObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../RenderTarget.hpp"
#include "../Texture/Texture2D.hpp"

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
        
        /// Get the clipping plane for refractions.
        /**
         * @return The clipping plane
         */
        glm::vec4 RefractionClippingPlane() const;
        
        /// Get the clipping plane for reflections.
        /**
         * @return The clipping plane
         */
        glm::vec4 ReflectionClippingPlane() const;
        
        /// Set how much the water textures should repat.
        /**
         * @param textureRepeat How many times the water textures should repeat.
         */
        void SetTextureRepeat(const glm::vec2& textureRepeat);
        
    private:
        // Water shaders.
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        // Render targets.
        RenderTarget* refractionTarget;
        RenderTarget* reflectionTarget;
        
        // Textures.
        Texture2D* dudvMap;
        glm::vec2 textureRepeat;
        
};

/** @} */
