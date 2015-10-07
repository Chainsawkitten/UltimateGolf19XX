#pragma once

#include <glm/glm.hpp>

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
            
            /// Position in the world.
            glm::vec3 position;
            
            /// Radius.
            float radius;
	};
}

/** @} */
