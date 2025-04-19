#ifndef FUNC_H_
#define FUNC_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <map>

#define SLOPE_TILE_DOWN 146
#define BLANK_TILE 0
#define TileSize 32
#define Scale 2.5
#define MapX 200
#define MapY 20

static SDL_Window* window = NULL ;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;

const int game_w = TileSize * 16 ;
const int game_h = TileSize * 9;
const int fps = 60;



typedef struct EnemySpawnPoint {
   int posx;
   int posy;
   bool spawned = false;
};

typedef struct Input
{
    int left = 0;
    int right = 0;
    int attack = 0;
    int jump = 0;
} ;

typedef struct MapObject
{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MapY][MapX];
};

    SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) ;

#endif // FUNC_H_
