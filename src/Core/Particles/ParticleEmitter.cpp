#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"

ParticleEmitter::ParticleEmitter(double minEmitTime, double maxEmitTime, bool relative) {
    this->minEmitTime = minEmitTime;
    this->maxEmitTime = maxEmitTime;
    this->relative = relative;
    timeToNext = minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
}

void ParticleEmitter::Update(double time, ParticleSystem* particleSystem, const Camera* camera) {
    this->particleSystem = particleSystem;
    this->camera = camera;
    
    timeToNext -= time;
    while (timeToNext < 0.0) {
        timeToNext += minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
        EmitParticle();
    }
}

void ParticleEmitter::EmitParticleAt(glm::vec3 position) {
    if (relative)
        position += camera->Position();
    
    particleSystem->EmitParticle(position);
}
