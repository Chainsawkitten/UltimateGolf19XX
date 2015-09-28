#include "VorbisFile.hpp"
#include "../Util/Log.hpp"
#include <stb_vorbis.c>

VorbisFile::VorbisFile(const char *filename) {
	Log() << "Warning!! Vorbis support not active!!\n";
    int channels;
    dataSize = stb_vorbis_decode_filename(filename, &channels, &sampleRate, reinterpret_cast<short**>(&data));
    
    if (dataSize == -1)
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
    
    if (channels > 1)
        format = AL_FORMAT_STEREO16;
    else
        format = AL_FORMAT_MONO16;
}

VorbisFile::~VorbisFile() {
    delete[] data;
}

const char* VorbisFile::Data() const {
    return data;
}

ALsizei VorbisFile::Size() const {
    return dataSize;
}

ALenum VorbisFile::Format() const {
    return format;
}

ALsizei VorbisFile::SampleRate() const {
    return sampleRate;
}
