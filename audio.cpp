
#include "audio.h"


AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    cleanup();
}


void AudioManager::loadMusic(const std::string& name, const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cout << "Failed to load music " << name << ": " << Mix_GetError() << std::endl;
        return;
    }
    musics[name] = music;
}

void AudioManager::playMusic(const std::string& name, int loops) {
    auto it = musics.find(name);
    if (it != musics.end()) {
        Mix_VolumeMusic(80);
        Mix_PlayMusic(it->second, loops);
    }
}

void AudioManager::stopMusic() {
    Mix_HaltMusic();
}

void AudioManager::loadSound(const std::string& name, const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        std::cout << "Failed to load sound " << name << ": " << Mix_GetError() << std::endl;
        return;
    }

    if ( name == "step") Mix_VolumeChunk(sound, 80);

    sounds[name] = sound;
}

void AudioManager::playSound(const std::string& name, int loops) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void AudioManager::cleanup() {
    for (auto& m : musics)
        Mix_FreeMusic(m.second);
    musics.clear();

    for (auto& s : sounds)
        Mix_FreeChunk(s.second);
    sounds.clear();

    Mix_CloseAudio();
}
