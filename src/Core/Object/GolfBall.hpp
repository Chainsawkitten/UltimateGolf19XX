#pragma once

#include "../Geometry/OBJModel.hpp"
#include "ModelObject.hpp"
#include "../Physics/Sphere.hpp"
#include "TerrainObject.hpp"

/** @ingroup Core
 * @{
 */
struct ClubType{
	float mass;
	float loft;
};

/// A golf ball that can be struck.
class GolfBall : public ModelObject {
    public:
        /// The type of ball.
        enum BallType {
            TWOPIECE, ///< Two-piece ball.
            THREEPIECE ///< Three-piece ball.
        };
        
        /// Create new golf ball.
        /**
         * @param ballType The type of ball to be created.
		 * @param terrain Reference to the terrain with which the ball will interact
         */
        GolfBall(BallType ballType, TerrainObject* terrain);
        
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
         * @param clippingPlane Clipping plane equation.
         */
		void Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const;
        
        /// Hit the golf ball with a club.
		void Strike(ClubType club, glm::vec3 clubVelocity);
        
        /// Set the radius of the golf ball.
        /**
         * @param radius Radius in meters.
         */
        void SetRadius(float radius);

		/// Resets ball to original position.
		void Reset();
		
    private:
		TerrainObject* terrain;
		float groundLevel;
        glm::vec3 velocity;
		glm::vec3 angularVelocity;
        bool active;
        Geometry::OBJModel* modelGeometry;
        
		Physics::Sphere sphere;
		float restitution;
        float mass;
        float area;
        BallType ballType;
};

/** @} */
