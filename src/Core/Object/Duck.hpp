#pragma once

#include "Object.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Geometry/OBJModel.hpp"
#include "../Camera/Camera.hpp"
#include "TerrainObject.hpp"
#include "Water.hpp"

/** @ingroup Core
 * @{
 */

/// A quacking duck.
class Duck : public Object {
    public:
        /// Create new duck.
        Duck();
        
        /// Destructor.
        ~Duck();
        
        /// Update the duck.
        /**
         * @param time Time since last frame (in seconds).
         * @param terrain %Terrain to check collision against.
         * @param water %Water to try to stay on.
         */
        void Update(double time, TerrainObject* terrain, Water* water);
        
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
        
        Geometry::OBJModel* geometry;
        Texture2D* texture;
        
        float velocity;
        float targetVelocity;
        float angularVelocity;
        float targetAngularVelocity;
        float lastChanged;
};

/** @} */
