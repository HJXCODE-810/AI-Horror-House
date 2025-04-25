#pragma once

#include <string>
#include <unordered_map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Initialize the audio system (OpenAL or platform-specific)
    bool initialize();

    // Load a sound file (e.g., into a buffer)
    unsigned int loadSound(const std::string& filename);

    // Play a sound once (non-positional)
    void playSound(unsigned int soundId);

    // Play a looping ambient sound (background music)
    void playAmbientSound(unsigned int soundId, bool loop = true);

    // Play a sound at a specific 3D position (requires OpenAL)
    void playSoundAt(unsigned int soundId, float x, float y, float z);

    // Stop a specific sound or all sounds
    void stopSound(unsigned int soundId);
    void stopAllSounds();

    // Update listener position (usually the camera's position) for 3D audio
    void updateListenerPosition(float x, float y, float z, float lookX, float lookY, float lookZ);

    // Clean up audio resources
    void shutdown();

private:
    // Audio context and buffer management (OpenAL or other)
    void* audioContext;  // A pointer for platform-specific context (e.g., OpenAL context)

    // Store sound buffers and sources
    std::unordered_map<unsigned int, unsigned int> soundBuffers;  // Map sound IDs to OpenAL buffers
    std::unordered_map<unsigned int, unsigned int> soundSources;  // Map sound IDs to OpenAL sources

    // Helper to load a sound file into an OpenAL buffer (cross-platform)
    bool loadSoundToBuffer(const std::string& filename, unsigned int& bufferId);

    // Helper for platform-specific string conversion (Windows-only for now)
    bool convertToWideString(const char* narrowStr, wchar_t* wideStr, size_t wideStrSize);
};
