#include "SoundBuffer.hpp"
#include "SoundSystem.hpp"

SoundBuffer::SoundBuffer(SoundFile* soundFile) {
	// Create audio buffer.
	alGetError();
	alGenBuffers((ALuint)1, &buffer);
	SoundSystem::CheckError("Couldn't create buffers.");

	// Set the buffer data.
	alBufferData(buffer, soundFile->Format(), soundFile->Data(), soundFile->Size(), soundFile->SampleRate());
	SoundSystem::CheckError("Couldn't set buffer data.");
}

SoundBuffer::~SoundBuffer() {
	alDeleteBuffers(1, &buffer);
}

ALuint SoundBuffer::Buffer() const {
	return buffer;
}
