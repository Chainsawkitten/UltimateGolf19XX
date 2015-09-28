# Ultimate Golf 19XX
Ultimate golf game with exploding balls.

## Building
1. Clone the GitHub repository.
2. Run

        git submodule init
        git submodule update
3. Use CMake to generate make/project files for externals/Headerize.
4. Build Headerize and put it in your PATH.
5. Use CMake to generate make/project files for Ultimate Golf 19XX.
6. Build Ultimate Golf 19XX.

### OpenAL Soft
Double check that the appropriate backends were detected. CMake's output will identify which backends were enabled. For most systems, you will likely want to make sure ALSA, OSS, and PulseAudio were detected (if your target system uses them). For Windows, make sure DirectSound was detected.