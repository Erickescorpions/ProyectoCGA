#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <AL/al.h>
#include <AL/alc.h>

struct WavHeader {
    char riff[4];
    int chunkSize;
    char wave[4];
    char fmt[4];
    int subchunk1Size;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
    char data[4];
    int dataSize;
};

class AudioManager {
public:
    static void init();
    static void playBackgroundMusic(const std::string& filename);
    static void cleanup();
};

#endif // AUDIO_MANAGER_H
