#include "func.h"

bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        printf("Failed to load image %s: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Failed to create texture from %s: %s\n", path.c_str(), SDL_GetError());
    }

    return texture;
}
