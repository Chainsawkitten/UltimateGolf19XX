#pragma once

#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

/// A spotlight.
class Light {
    public:
        /// Create new light.
        Light();
        
        /// Position.
        glm::vec4 position;
        
        /// Intensity.
        glm::vec3 intensity;
        
        /// Diffuse coefficient.
        glm::vec3 diffuseCoefficient;
};

/** @} */
