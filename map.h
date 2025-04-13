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
    MapObject game_map;
    MapObject visual_map;

    bool load();

    //void loadTilesetsInfo();
    //void loadLayersInfo();

    void loadTextures(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void SaveCollision();

private:
    Tilebrick tileTextures[250];
    std::string mapFile;
    tson::Map mp;
};

#endif
