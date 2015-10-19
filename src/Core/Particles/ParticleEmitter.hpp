#pragma once

#include "../Object/Object.hpp"

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
         * @param follow %Object to follow (for relative emitting).
         */
        void Update(double time, ParticleSystem* particleSystem, const Object* follow);
        
        /// Emit particle.
        virtual void EmitParticle() = 0;

		/// Get lifetime
		double Lifetime();

		/// Reset lifetime
		void ResetLifetime();
        
    protected:
        void EmitParticleAt(glm::vec3 position);
        
    private:
        double minEmitTime;
        double maxEmitTime;
        double timeToNext;
        
        bool relative;

		double lifetime;
        
        ParticleSystem* particleSystem;
        const Object* follow;
};

/** @} */
