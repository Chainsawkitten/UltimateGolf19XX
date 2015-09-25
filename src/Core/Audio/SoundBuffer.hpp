#pragma once

#include "SoundFile.hpp"

/** @ingroup Core
 * @{
 */

/// Wrapper for OpenAL buffers.
class SoundBuffer {
	public:
		/// Create a sound buffer from a sound file.
		/**
		 * @param soundFile The sound file containing the sound.
		 */
		SoundBuffer(SoundFile* soundFile);

		/// Destructor
		~SoundBuffer();

		/// Get AL buffer.
		/**
		 * @return The OpenAL buffer ID.
		 */
		ALuint Buffer() const;

	private:
		ALuint buffer;
};

/** @} */
