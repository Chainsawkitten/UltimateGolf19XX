#include "Sound.hpp"
#include <AL/alc.h>
#include "SoundSystem.hpp"

Sound::Sound(SoundBuffer* buffer) {
    position = glm::vec3(0.f, 0.f, 0.f);
    pitch = 1.f;
    gain = 1.f;
    loop = AL_FALSE;
    
	alGenSources((ALuint)1, &source);

	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcei(source, AL_LOOPING, loop);

	alSourcei(source, AL_BUFFER, buffer->Buffer());
	SoundSystem::CheckError("Couldn't set sound source buffer.");
}

Sound::~Sound() {
	alDeleteSources(1, &source);
}

const glm::vec3& Sound::Position() const {
	return position;
}

void Sound::SetPosition(const glm::vec3& position) {
	this->position = position;
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}

ALfloat Sound::Pitch() const {
	return pitch;
}

void Sound::SetPitch(ALfloat pitch) {
	this->pitch = pitch;
	alSourcef(source, AL_PITCH, pitch);
}

ALfloat Sound::Gain() const {
	return gain;
}

void Sound::SetGain(ALfloat gain) {
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain);
}

ALboolean Sound::Looping() const {
	return loop;
}

void Sound::SetLooping(ALboolean loop) {
	this->loop = loop;
	alSourcei(source, AL_LOOPING, loop);
}

void Sound::Play() {
	alSourcePlay(source);
	SoundSystem::CheckError("Couldn't play sound.");
}

void Sound::Pause() {
	alSourcePause(source);
	SoundSystem::CheckError("Couldn't pause sound.");
}

void Sound::Stop() {
	alSourceStop(source);
	SoundSystem::CheckError("Couldn't pause sound.");
}

ALint Sound::State() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state;
}
