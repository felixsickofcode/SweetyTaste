
#include "player.h"

Player::Player() : x(100), y(500), velX(0), velY(0) {}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: velX = -1; break;
            case SDLK_RIGHT: velX = 1; break;
            case SDLK_UP: velY = -2; break;
        }
    }
    if (e.type == SDL_KEYUP) {
        velX = 0;
        velY = 0;
    }
}

void Player::update() {
    x += velX;
    y += velY;
    if (y > 500) y = 500;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, 50, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
