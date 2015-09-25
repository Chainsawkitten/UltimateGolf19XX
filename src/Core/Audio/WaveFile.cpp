#include "WaveFile.hpp"
#include <fstream>
#include <cstring>
#include "../Util/Log.hpp"

WaveFile::WaveFile(const char* filename) {
	std::ifstream waveFile;
	waveFile.open(filename, std::ios::binary);
	if (!waveFile.is_open())
        Log() << "Couldn't open wave file " << filename << " for reading.\n";

	// Read RIFF.
	char id[4];
	waveFile.read(id, 4);
	if (!strcmp(id, "RIFF"))
		Log() << "File is not a RIFF file.\n";

	// Read file size (excluding RIFF).
	unsigned long size;
	waveFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// Read WAVE.
	waveFile.read(id, 4);
	if (!strcmp(id, "WAVE"))
		Log() << "File is not a wave file.\n";

	// Read format.
	unsigned long formatLength, avgBytesPerSec;
	short formatTag, channels, blockAlign, bitsPerSample;

	waveFile.read(id, 4);
	waveFile.read(reinterpret_cast<char*>(&formatLength), sizeof(formatLength));
	waveFile.read(reinterpret_cast<char*>(&formatTag), sizeof(formatTag));
	waveFile.read(reinterpret_cast<char*>(&channels), sizeof(channels));
	waveFile.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
	waveFile.read(reinterpret_cast<char*>(&avgBytesPerSec), sizeof(avgBytesPerSec));
	waveFile.read(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
	waveFile.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

	// Read data.
	waveFile.read(id, 4);
	waveFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
	data = new char[dataSize];
	waveFile.read(data, dataSize);

	waveFile.close();

	format = ToALFormat(channels, bitsPerSample);
}

WaveFile::~WaveFile() {
	delete[] data;
}

const char* WaveFile::Data() const {
	return data;
}

ALsizei WaveFile::Size() const {
	return dataSize;
}

ALenum WaveFile::Format() const {
	return format;
}

ALsizei WaveFile::SampleRate() const {
	return sampleRate;
}

ALenum WaveFile::ToALFormat(short channels, short bitsPerSample) {
	bool stereo = (channels > 1);

	switch (bitsPerSample) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}
