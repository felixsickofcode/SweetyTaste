
#ifndef AUDIO_H
#define AUDIO_H

#include "func.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool init();
    void loadMusic(const std::string& name, const std::string& path);
    void playMusic(const std::string& name, int loops = -1);
    void stopMusic();

    void loadSound(const std::string& name, const std::string& path);
    void playSound(const std::string& name, int loops = 0);

    void cleanup();

private:
    std::map<std::string, Mix_Music*> musics;
    std::map<std::string, Mix_Chunk*> sounds;
};

#endif
