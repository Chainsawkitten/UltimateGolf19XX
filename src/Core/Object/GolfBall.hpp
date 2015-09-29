#pragma once

#include "Object.hpp"
#include "../Geometry/Model.hpp"
#include "ModelObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"

/** @ingroup Core
 * @{
 */

/// A golf ball that can be struck.
class GolfBall : public Object {
    public:
        /// Create new golf ball.
        GolfBall();
        
        /// Destructor.
        ~GolfBall();
        
        /// Update movement.
        /**
         * @param time Time since last frame (in seconds).
         */
        void Update(double time);
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
        /// Hit the golf ball with a club.
        void Strike();
        
    private:
		ModelObject* modelObject;
		Geometry::Model* modelGeometry;
        glm::vec3 velocity;
        bool active;
};

/** @} */
