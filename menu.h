#ifndef MENU_H
#define MENU_H

#include "func.h"

class Menu
{
public:
    Menu();
    ~Menu();

    SDL_Texture* RenderTextToTexture(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color);
    int Show(SDL_Renderer* renderer);
    SDL_Rect btn1, btn2, btn3, game_title, line;
    TTF_Font* font;
    TTF_Font* title_font;
    SDL_Color textColor;
    bool IsMouseOverButton(const SDL_Rect& button);
    void DrawFrame(SDL_Renderer* renderer, SDL_Texture* tex, const SDL_Rect& rect);
private:
    SDL_Texture* frameTexture;
    void DrawText(SDL_Renderer* renderer, const std::string& text, SDL_Rect& button);
    void DrawTitle(SDL_Renderer* renderer, const std::string& text, SDL_Rect& button);
    void Draw(SDL_Renderer* renderer);
    int HandleEvent(SDL_Event& e);
};

#endif
