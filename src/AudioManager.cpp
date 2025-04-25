#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <AL/al.h>
#include <AL/alc.h>

AudioManager::AudioManager() : audioContext(nullptr) {}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    std::cout << "Initializing Audio Manager..." << std::endl;

    // OpenAL initialization
    ALCdevice* device = alcOpenDevice(nullptr);  // Open default device
    if (!device) {
        std::cerr << "Failed to open OpenAL device!" << std::endl;
        return false;
    }

    audioContext = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(static_cast<ALCcontext*>(audioContext));

    if (!audioContext) {
        std::cerr << "Failed to create OpenAL context!" << std::endl;
        alcCloseDevice(device);
        return false;
    }

    std::cout << "Audio Manager Initialized (OpenAL)" << std::endl;
    return true;
}

unsigned int AudioManager::loadSound(const std::string& filename) {
    unsigned int bufferId;
    if (loadSoundToBuffer(filename, bufferId)) {
        unsigned int soundId = soundBuffers.size() + 1;
        soundBuffers[soundId] = bufferId;
        return soundId;
    } else {
        std::cerr << "Failed to load sound file: " << filename << std::endl;
        return 0;
    }
}

bool AudioManager::loadSoundToBuffer(const std::string& filename, unsigned int& bufferId) {
    // Logic for loading the sound file into OpenAL buffer
    // For simplicity, assume this step uses raw sound data loading

    // Check if the file exists
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Sound file not found: " << filename << std::endl;
        return false;
    }
    file.close();

    alGenBuffers(1, &bufferId);
    // Here, you would normally load the sound data from the file into the buffer
    // Example: alBufferData(bufferId, AL_FORMAT_MONO16, data, size, frequency);

    return true;
}

void AudioManager::playSound(unsigned int soundId) {
    if (soundBuffers.find(soundId) != soundBuffers.end()) {
        unsigned int source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, soundBuffers[soundId]);
        alSourcePlay(source);
        soundSources[soundId] = source;
    } else {
        std::cerr << "Sound ID not found: " << soundId << std::endl;
    }
}

void AudioManager::playAmbientSound(unsigned int soundId, bool loop) {
    if (soundBuffers.find(soundId) != soundBuffers.end()) {
        unsigned int source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, soundBuffers[soundId]);
        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(source);
        soundSources[soundId] = source;
    } else {
        std::cerr << "Sound ID not found: " << soundId << std::endl;
    }
}

void AudioManager::playSoundAt(unsigned int soundId, float x, float y, float z) {
    if (soundBuffers.find(soundId) != soundBuffers.end()) {
        unsigned int source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, soundBuffers[soundId]);
        alSource3f(source, AL_POSITION, x, y, z);
        alSourcePlay(source);
        soundSources[soundId] = source;
    } else {
        std::cerr << "Sound ID not found: " << soundId << std::endl;
    }
}

void AudioManager::stopSound(unsigned int soundId) {
    if (soundSources.find(soundId) != soundSources.end()) {
        alSourceStop(soundSources[soundId]);
        alDeleteSources(1, &soundSources[soundId]);
        soundSources.erase(soundId);
    } else {
        std::cerr << "Sound source not found for ID: " << soundId << std::endl;
    }
}

void AudioManager::stopAllSounds() {
    for (auto& [id, source] : soundSources) {
        alSourceStop(source);
        alDeleteSources(1, &source);
    }
    soundSources.clear();
}

void AudioManager::updateListenerPosition(float x, float y, float z, float lookX, float lookY, float lookZ) {
    ALfloat listenerPos[] = {x, y, z};
    ALfloat listenerOri[] = {lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f};  // Forward, Up direction
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void AudioManager::shutdown() {
    std::cout << "Shutting down Audio Manager..." << std::endl;

    stopAllSounds();

    // Clean up OpenAL
    if (audioContext) {
        ALCdevice* device = alcGetContextsDevice(static_cast<ALCcontext*>(audioContext));
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(static_cast<ALCcontext*>(audioContext));
        alcCloseDevice(device);
    }

    std::cout << "Audio Manager Shutdown Complete" << std::endl;
}

bool AudioManager::convertToWideString(const char* narrowStr, wchar_t* wideStr, size_t wideStrSize) {
    // Windows-specific string conversion (for PlaySound if needed)
    #ifdef _WIN32
    int result = MultiByteToWideChar(CP_ACP, 0, narrowStr, -1, wideStr, static_cast<int>(wideStrSize));
    if (result == 0) {
        std::cerr << "MultiByteToWideChar failed. Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
    #else
    std::cerr << "convertToWideString is Windows-only." << std::endl;
    return false;
    #endif
}
