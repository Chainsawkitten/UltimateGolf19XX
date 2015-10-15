#pragma once

#include <glm/glm.hpp>
#include "Triangle.hpp"

/** @ingroup Core
 * @{
 */

namespace Physics {
    /// Sphere.
	class Sphere {
        public:
            /// Create new sphere.
            /**
             * @param origin The sphere's origin.
             * @param radius The radius of the sphere.
             */
            Sphere(glm::vec3 origin = glm::vec3(0.f, 0.f, 0.f), float radius = 0.f);
            
			/// Sphere vs triangle intersection testing.
			/**
			 * @param triangle to test against
			 * @return if the sphere intersected triangle or not.
			 */
			bool SphereVsTriangle(Triangle triangle);

            /// Position in the world.
            glm::vec3 position;
            
            /// Radius.
            float radius;
	};
}

/** @} */
