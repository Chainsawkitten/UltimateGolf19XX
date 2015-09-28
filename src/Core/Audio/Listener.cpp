#include "Listener.hpp"
#include <AL/al.h>
#include "SoundSystem.hpp"

Listener::Listener() {
    position = glm::vec3(0.f, 0.f, 0.f);
    forward = glm::vec3(0.f, 0.f, -1.f);
    up = glm::vec3(0.f, 1.f, 0.f);

    horizontalAngle = 0.f;
    verticalAngle = 0.f;
    tiltAngle = 0.f;
    
    gain = 1.f;
}

const glm::vec3& Listener::Position() const {
	return position;
}

void Listener::SetPosition(const glm::vec3& position) {
	this->position = position;
	alListener3f(AL_POSITION, position.x, position.y, position.z);
	SoundSystem::CheckError("Couldn't set listener position.");
}

const glm::vec3& Listener::Forward() const {
	return forward;
}

const glm::vec3& Listener::Up() const {
	return up;
}

void Listener::SetOrientation(const glm::vec3& forward, const glm::vec3& up) {
    this->forward = forward;
    this->up = up;
    
	ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, listenerOri);
	SoundSystem::CheckError("Couldn't set listener orientation.");
}

float Listener::Gain() const {
    return gain;
}

void Listener::SetGain(float gain) {
    this->gain = gain;
    alListenerf(AL_GAIN, gain);
}
