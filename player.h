
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
    Player();
    void handleEvent(SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);
private:
    int x, y, velX, velY;
};

#endif
