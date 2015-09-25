#include "SoundSystem.hpp"
#include "../Util/Log.hpp"
#include <AL/al.h>

SoundSystem* SoundSystem::instance = nullptr;

SoundSystem::SoundSystem() {
	// Open default audio device.
	device = alcOpenDevice(nullptr);
	if (!device)
		Log() << "Couldn't open default audio device.\n";

	// Create audio context.
	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context))
		Log() << "Couldn't create audio context.\n";

	listener = new Listener();
	instance = this;
}

SoundSystem::~SoundSystem() {
	delete listener;

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

Listener* SoundSystem::GetListener() const {
	return listener;
}

SoundSystem* SoundSystem::GetInstance() {
	return instance;
}

void SoundSystem::CheckError(const char* message) {
	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		Log() << message << "\n";
		if (error == AL_INVALID_NAME) Log() << "Invalid name\n";
		if (error == AL_INVALID_ENUM) Log() << "Invalid enum\n";
		if (error == AL_INVALID_VALUE) Log() << "Invalid value\n";
		if (error == AL_INVALID_OPERATION) Log() << "Invalid operation\n";
		if (error == AL_OUT_OF_MEMORY) Log() << "Out of memory like!\n";
	}
}
