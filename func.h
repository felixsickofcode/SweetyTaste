#ifndef FUNC_H_
#define FUNC_H_

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

#define BLANK_TILE 0
#define TileSize 32
#define Scale 2.5
#define MapX 200
#define MapY 8

#define OffsetY 17
#define OffsetX 40
#define MaxFallSpeed 2
#define PlayerSpeed 2.5

static SDL_Window* window = NULL ;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;

const int game_w = TileSize * 16 ;
const int game_h = TileSize * 9;
const int fps = 60;

typedef struct Input
{
    int left;
    int right;
    int up;
    int down;
    int dash;
    int jump;
};

typedef struct MapObject
{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MapY][MapX];
};
namespace CommonFunc
{
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) ;
}
#endif // FUNC_H_
