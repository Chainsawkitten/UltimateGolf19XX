#pragma once

#include "Object.hpp"
#include "../Geometry/Model.hpp"
#include "ModelObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Physics/Sphere.hpp"

/** @ingroup Core
 * @{
 */

/// A golf ball that can be struck.
class GolfBall : public Object {
    public:
        /// The type of ball.
        enum BallType {
            TWOPIECE, ///< Two-piece ball.
            THREEPIECE ///< Three-piece ball.
        };

		struct ClubType{
			float mass;
			float loft;
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
		void Strike(float clubMass, float clubLoft, glm::vec3 clubVelocity);
        
        /// Set the radius of the golf ball.
        /**
         * @param radius Radius in meters.
         */
        void SetRadius(float radius);
        
    private:
		ModelObject* modelObject;
		Geometry::Model* modelGeometry;
        glm::vec3 velocity;
		glm::vec3 angularVelocity;
        bool active;
        
		Physics::Sphere* sphere;
		float resistution;
        float mass;
        float area;
        BallType ballType;
};

/** @} */
