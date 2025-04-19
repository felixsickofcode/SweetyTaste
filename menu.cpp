#include "menu.h"

Menu::Menu()
{
    game_title  = {game_w / 2 - 150, 0 , 300, 150};
    btn1 = {game_w / 2 - 60, game_h - 140, 120, 30};
    btn2 = {game_w / 2 - 60, game_h - 100, 120, 30};
    btn3 = {game_w / 2 - 60, game_h - 60, 120, 30};
    line = { game_w / 2 - 60, 130, 120, 1};
    font = TTF_OpenFont("asset/Font/Font2.ttf", 80);
    title_font = TTF_OpenFont("asset/Font/Font2.ttf", 350);
    textColor = {255, 255, 255}; // Màu sắc chữ
}

Menu::~Menu()
{
    TTF_CloseFont(font);
}

SDL_Texture* Menu::RenderTextToTexture(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cout << "Error creating text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    return textTexture;
}
void Menu::DrawFrame(SDL_Renderer* renderer, SDL_Texture* tex, const SDL_Rect& rect)
{
    int border = 16;

    SDL_Rect src[9], dst[9];

    src[0] = {0, 0, border, border};
    src[1] = {border, 0, 16, border};
    src[2] = {border * 2, 0, border, border};

    src[3] = {0, border, border, 16};
    src[4] = {border, border, 16, 16};
    src[5] = {border * 2, border, border, 16};

    src[6] = {0, border * 2, border, border};
    src[7] = {border, border * 2, 16, border};
    src[8] = {border * 2, border * 2, border, border};

    border = 8;
    int w = rect.w - 2 * border;
    int h = rect.h - 2 * border;

    dst[0] = {rect.x , rect.y , border, border}; // top-left
    dst[1] = {rect.x + border, rect.y , w, border }; // top
    dst[2] = {rect.x + w + border , rect.y, border, border}; // top-right

    dst[3] = {rect.x , rect.y + border, border, h }; // left
    dst[4] = {rect.x, rect.y, w, h}; // center
    dst[5] = {rect.x + w + border, rect.y + border, border, h}; // right

    dst[6] = {rect.x , rect.y + h  + border, border, border}; // bottom-left
    dst[7] = {rect.x + border, rect.y + h + border, w, border}; // bottom
    dst[8] = {rect.x + w + border, rect.y + h + border, border, border}; // bottom-right

    for (int i = 0; i < 9; ++i)
        SDL_RenderCopy(renderer, tex, &src[i], &dst[i]);
}

bool Menu::IsMouseOverButton(const SDL_Rect& button) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseX /= Scale;
    mouseY /= Scale;
    return mouseX >= button.x && mouseX <= button.x + button.w &&
           mouseY >= button.y && mouseY <= button.y + button.h;
}
void Menu::Draw(SDL_Renderer* renderer)
{
    frameTexture = LoadTexture( "asset/panel-border-010.png", renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &line);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 0);
    SDL_RenderFillRect(renderer, &btn1);
    SDL_RenderFillRect(renderer, &btn2);
    SDL_RenderFillRect(renderer, &btn3);

    SDL_RenderFillRect(renderer, &game_title);
    DrawTitle(renderer, "MYGAME", game_title);

    if (IsMouseOverButton(btn1)) DrawFrame(renderer, frameTexture, btn1);
    DrawText(renderer, "level 1", btn1);
    if (IsMouseOverButton(btn2)) DrawFrame(renderer, frameTexture, btn2);
    DrawText(renderer, "level 2", btn2);
    if (IsMouseOverButton(btn3)) DrawFrame(renderer, frameTexture, btn3);
    DrawText(renderer, "level 3", btn3);

    SDL_RenderPresent(renderer);
}

void Menu::DrawText(SDL_Renderer* renderer, const std::string& text, SDL_Rect& button)
{
    SDL_Texture* textTexture = RenderTextToTexture(renderer, text, font, textColor);
    if (!textTexture) return;

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    textHeight /= 4;
    textWidth /=4;
    SDL_Rect textRect = {button.x + (button.w - textWidth) / 2, button.y + (button.h - textHeight) / 2, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}
void Menu::DrawTitle(SDL_Renderer* renderer, const std::string& text, SDL_Rect& button)
{
    SDL_Texture* textTexture = RenderTextToTexture(renderer, text, title_font, textColor);
    if (!textTexture) return;

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    textHeight /= 4;
    textWidth /=4;
    SDL_Rect textRect = {button.x + (button.w - textWidth) / 2, button.y + (button.h - textHeight) / 2, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}
int Menu::HandleEvent(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = e.button.x / Scale;
        int y = e.button.y / Scale;
        SDL_Point p = {x, y};

        if (SDL_PointInRect(&p, &btn1)) return 1;
        if (SDL_PointInRect(&p, &btn2)) return 2;
        if (SDL_PointInRect(&p, &btn3)) return 3;

        std::cout << "Click outside button region: (" << x << ", " << y << ")\n";
    }
    return 0;
}

int Menu::Show(SDL_Renderer* renderer)
{
    SDL_Event e;
    int level = 0;

    while (level == 0)
    {
        Draw(renderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return -1;

            level = HandleEvent(e);
        }
    }

    return level;
}
