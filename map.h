#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "tileson.hpp"
#include <string>
#include <vector>

class Map {
public:
    Map(const std::string& filename);
    bool load();
    void loadTilesetsInfo();
    void loadLayersInfo();
    void loadTextures(SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
    void test(SDL_Renderer* renderer);
private:
    std::string mapFile;
    tson::Map mp;
    std::unordered_map<uint32_t, SDL_Texture*> tileTextures;

};

#endif
