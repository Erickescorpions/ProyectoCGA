#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

ALCdevice* device = nullptr;
ALCcontext* context = nullptr;
ALuint source;
ALuint buffer;

bool loadWavFile(const std::string& filename, ALuint& buffer);

void AudioManager::init() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Error: Cannot open OpenAL device" << std::endl;
        exit(-1);
    }

    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context)) {
        std::cerr << "Error: Cannot create OpenAL context" << std::endl;
        alcCloseDevice(device);
        exit(-1);
    }

    alGenSources(1, &source);
    std::cout << "OpenAL initialized successfully" << std::endl;
}

void AudioManager::playBackgroundMusic(const std::string& filename) {
    // Detener el audio actual si está en reproducción
    alSourceStop(source);

    // Liberar el buffer actual si ya existe
    if (buffer != 0) {
        alDeleteBuffers(1, &buffer);
    }
    
    if (!loadWavFile(filename, buffer)) {
        std::cerr << "Error: Unable to load WAV file" << std::endl;
        return;
    }

    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE); // Enable looping
    alSourcef(source, AL_GAIN, 1.0f);       // Volume
    alSourcePlay(source);

    if (alGetError() != AL_NO_ERROR) {
        std::cerr << "Error: Failed to play background music" << std::endl;
    } else {
        std::cout << "Background music is playing" << std::endl;
    }
}

void AudioManager::cleanup() {
    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(nullptr);
    if (context) alcDestroyContext(context);
    if (device) alcCloseDevice(device);
    std::cout << "OpenAL cleaned up successfully" << std::endl;
}

bool loadWavFile(const std::string& filename, ALuint& buffer) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open WAV file: " << filename << std::endl;
        return false;
    }

    WavHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    if (std::strncmp(header.riff, "RIFF", 4) != 0 || std::strncmp(header.wave, "WAVE", 4) != 0) {
        std::cerr << "Error: Invalid WAV file format" << std::endl;
        return false;
    }

    ALenum format = (header.numChannels == 1) ? 
        (header.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16) : 
        (header.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);

    if (format == 0) {
        std::cerr << "Error: Unsupported bit depth or channel count" << std::endl;
        return false;
    }

    std::vector<char> data(header.dataSize);
    file.read(data.data(), header.dataSize);

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data.data(), header.dataSize, header.sampleRate);

    if (alGetError() != AL_NO_ERROR) {
        std::cerr << "Error: Failed to buffer WAV data" << std::endl;
        return false;
    }

    std::cout << "WAV file loaded successfully" << std::endl;
    std::cout << "Formato del archivo: " << header.audioFormat << "\n";
    std::cout << "Canales: " << header.numChannels << "\n";
    std::cout << "Tasa de muestreo: " << header.sampleRate << "\n";
    std::cout << "Tamaño de datos: " << header.dataSize << "\n";
    return true;

    
}
