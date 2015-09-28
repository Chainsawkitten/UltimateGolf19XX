#pragma once

#include "../Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

class ParticleSystem;

/// Emitter that emits particles.
class ParticleEmitter {
    public:
        /// Create new particle emitter.
        /**
         * @param minEmitTime The minimum time between each emitted particle.
         * @param maxEmitTime The maximum time between each emitted particle.
         * @param relative Whether to emit particles relative to the camera position.
         */
        ParticleEmitter(double minEmitTime, double maxEmitTime, bool relative);
        
        /// Spawn new particles.
        /**
         * @param time Time since last frame (in seconds).
         * @param particleSystem Particle system to add particles to.
         * @param camera Camera (for camera-relative emitting).
         */
        void Update(double time, ParticleSystem* particleSystem, const Camera* camera);
        
        /// Emit particle.
        virtual void EmitParticle() = 0;
        
    protected:
        void EmitParticleAt(glm::vec3 position);
        
    private:
        double minEmitTime;
        double maxEmitTime;
        double timeToNext;
        
        bool relative;
        
        ParticleSystem* particleSystem;
        const Camera* camera;
};

/** @} */
