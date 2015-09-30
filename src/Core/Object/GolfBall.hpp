#pragma once

#include "Object.hpp"
#include "../Geometry/Model.hpp"
#include "ModelObject.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Physics/Sphere.hpp"
#include "TerrainObject.hpp"

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
         */
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
        /// Hit the golf ball with a club.
		void Strike(ClubType club, glm::vec3 clubVelocity);
        
        /// Set the radius of the golf ball.
        /**
         * @param radius Radius in meters.
         */
        void SetRadius(float radius);

		/// List of all available clubtypes
		ClubType wood1 = ClubType{ 0.2f, glm::radians<float>(11.f) };
		ClubType wood3 = ClubType{ 0.208f, glm::radians<float>(15.f) };
		ClubType wood5 = ClubType{ 0.218f, glm::radians<float>(18.f) };
		ClubType iron2 = ClubType{ 0.232f, glm::radians<float>(18.f) };
		ClubType iron3 = ClubType{ 0.239f, glm::radians<float>(21.f) };
		ClubType iron4 = ClubType{ 0.246f, glm::radians<float>(24.f) };
		ClubType iron5 = ClubType{ 0.253f, glm::radians<float>(27.f) };
		ClubType iron6 = ClubType{ 0.260f, glm::radians<float>(31.f) };
		ClubType iron7 = ClubType{ 0.267f, glm::radians<float>(35.f) };
		ClubType iron8 = ClubType{ 0.274f, glm::radians<float>(39.f) };
		ClubType iron9 = ClubType{ 0.281f, glm::radians<float>(43.f) };
		ClubType pitchingWedge = ClubType{ 0.285f, glm::radians<float>(48.f) };
		ClubType sandWedge = ClubType{ 0.296f, glm::radians<float>(55.f) };
		ClubType putter = ClubType{ 0.33f, glm::radians<float>(4.f) };

    private:
		ModelObject* modelObject;
		TerrainObject* terrain;
		float groundLevel;
		Geometry::Model* modelGeometry;
        glm::vec3 velocity;
		glm::vec3 angularVelocity;
        bool active;
        
		Physics::Sphere sphere;
		float restitution;
        float mass;
        float area;
        BallType ballType;
};

/** @} */
