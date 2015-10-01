#pragma once

#include "GeometryObject.hpp"
#include <string>
#include "../Geometry/Model.hpp"
#include "../Texture/Texture2D.hpp"

/** @ingroup Core
 * @{
 */

/// A game object with a renderable model. 
class ModelObject : public GeometryObject {
    public:
        /// Create new modelObject
        /**
         * @param model Model to load geometry from.
         * @param diffusePath Path to diffuse texture.
         * @param normalPath Path to normal texture.
         * @param specularPath Path to specular texture.
         */
        ModelObject(const Geometry::Model* model, std::string diffusePath, std::string normalPath, std::string specularPath);
    
        /// Destructor
        ~ModelObject();
    
        /// Render the object.
        /**
         * @param shaderProgram %Shader program to render the object with.
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         * @param clippingPlane Clipping plane equation.
         */
        void Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const;
        
    private:
        // Shaders.
        Shader* vertexShader;
        Shader* fragmentShader;
        Shader* geometryShader;
        ShaderProgram* shaderProgram;
        
        // Textures.
        Texture2D* diffuse;
        Texture2D* normal;
        Texture2D* specular;
};

/** @} */
