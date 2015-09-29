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
        /// The type of ball.
        enum BallType {
            TWOPIECE, ///< Two-piece ball.
            THREEPIECE ///< Three-piece ball.
        };
        
        /// Create new golf ball.
        /**
         * @param ballType The type of ball to be created.
         */
        GolfBall(BallType ballType);
        
        /// Destructor.
        ~GolfBall();
        
        /// Update movement.
        /**
         * @param time Time since last frame (in seconds).
         * @param wind The wind velocity.
         */
        void Update(double time, const glm::vec3& wind);
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
        /// Hit the golf ball with a club.
        void Strike();
        
        /// Set the radius of the golf ball.
        /**
         * @param radius Radius in meters.
         */
        void SetRadius(float radius);
        
    private:
        Texture2D* texture;
        
        // Shaders
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        glm::vec3 velocity;
        bool active;
        
        float mass;
        float radius;
        float area;
        BallType ballType;
};

/** @} */
