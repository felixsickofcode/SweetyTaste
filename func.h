#ifndef FUNC_H_
#define FUNC_H_

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

static SDL_Window* window = NULL ;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;

const int scale_ = 2;
const int game_w = 32 * 20 * scale_;
const int game_h = 32 * scale_ * 11;

typedef struct Input
{
    int left;
    int right;
    int up;
    int down;
    int dash;
    int jump;
};

#endif // FUNC_H_
