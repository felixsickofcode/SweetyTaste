#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "player.h"


class Game {
public:
    Game();
    ~Game();
    void run();
private:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    void processEvents();
    void update();
    void render();
};

#endif

