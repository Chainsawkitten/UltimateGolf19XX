#pragma once

#include "ParticleEmitter.hpp"
#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

class PointParticleEmitter : public ParticleEmitter {
    public:
        /// Create new PointParticleEmitter.
        /**
         * @param origin Origin.
         * @param minEmitTime The minimum time between each emitted particle.
         * @param maxEmitTime The maximum time between each emitted particle.
         */
        PointParticleEmitter(glm::vec3 origin, double minEmitTime, double maxEmitTime, bool relative = false);
        
        /// Emit particle.
        void EmitParticle();
        
    private:
        glm::vec3 origin;
};

/** @} */
