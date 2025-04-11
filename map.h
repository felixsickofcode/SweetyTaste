#ifndef MAP_H
#define MAP_H

#include "func.h"
#include "tileson.hpp"
#include "baseobj.h"
class Tilebrick : public baseobj
{
public:
    Tilebrick(){;}
    ~Tilebrick(){;}
};
class Map {
public:
    Map(const std::string& filename);
    bool load();
    //void loadTilesetsInfo();
    //void loadLayersInfo();
    void loadTextures(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
private:
    Tilebrick tileTextures[250];
    std::string mapFile;
    tson::Map mp;
    //std::unordered_map<uint32_t, SDL_Texture*> tileTextures;

};

#endif
