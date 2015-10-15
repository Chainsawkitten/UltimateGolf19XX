#pragma once

#include "../Geometry/OBJModel.hpp"
#include "ModelObject.hpp"
#include "../Physics/Sphere.hpp"
#include "TerrainObject.hpp"
#include "PlayerObject.hpp"
#include "Water.hpp"
#include <glm/gtc/quaternion.hpp>

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
        /// Golf ball's state.
        enum State {
            INITIAL, ///< Not struck.
            ACTIVE, ///< Struck, travelling.
            EXPLODED, ///< Has exploded.
            OUT ///< Ended up in water or outside level.
        };
        
        /// The type of ball.
        enum BallType {
            TWOPIECE, ///< Two-piece ball.
            THREEPIECE ///< Three-piece ball.
        };
        
        /// Create new golf ball.
        /**
         * @param ballType The type of ball to be created.
		 * @param terrain Reference to the terrain with which the ball will interact.
		 * @param water %Water.
         */
        GolfBall(BallType ballType, TerrainObject* terrain, Water* water);
        
        /// Destructor.
        ~GolfBall();
        
        /// Update movement.
        /**
         * @param time Time since last frame (in seconds).
         * @param wind The wind velocity.
		 * @param players Vector containing PlayerObjects.
         */
		void Update(double time, const glm::vec3& wind, std::vector<PlayerObject>& players);
        
        /// Render the object.
        /**
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         * @param clippingPlane Clipping plane equation.
         */
		void Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const;
        
        /// Hit the golf ball with a club.
		void Strike(ClubType club, const glm::vec3& clubVelocity);
        
        /// Set the radius of the golf ball.
        /**
         * @param radius Radius in meters.
         */
        void SetRadius(float radius);

		/// Resets ball to original position.
		void Reset();
		
		/// Explodes golfball
        /**
         * @param players Vector describing all players.
         */
		void Explode(std::vector<PlayerObject>& players);
        
        /// Get orientation matrix.
		/**
		 * @return Object's orientation matrix based on its rotation.
		 */
		glm::mat4 Orientation() const;
        
        /// Get the golf ball's current state.
        /**
         * @return The golf ball's current state.
         */
        State GetState() const;
		
    private:
		TerrainObject* terrain;
        Water* water;
        glm::vec3 velocity;
        
        glm::quat orientation;
		glm::vec3 angularVelocity;
		glm::vec3 origin;
        State state;
        Geometry::OBJModel* modelGeometry;
        
		Physics::Sphere sphere;
		float restitution;
        float mass;
        float area;
        BallType ballType;
};

/** @} */
