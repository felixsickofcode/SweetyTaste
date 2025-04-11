#include "map.h"
#include "func.h"
#include "baseobj.h"
#include "player.h"
bool inidata()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if ( ret < 0) return false;
    window = SDL_CreateWindow("SweetyTaste",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_w, game_h, SDL_WINDOW_SHOWN);
    if ( window == NULL) success = false;
    else
    {
        renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if ( renderer == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;

        }
    }
    SDL_RenderSetScale(renderer, scale_, scale_);
    return success;
}
void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}
int main(int argc, char* argv[])
{
    if (!inidata()) return -1;
{// Khởi tạo
//    if (SDL_Init(SDL_INIT_VIDEO) != 0)
//    {
//        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//    SDL_Window* window = SDL_CreateWindow("SweetyTaste",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_w, game_h, SDL_WINDOW_SHOWN);
//    if (!window)
//    {
//        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_RenderSetScale(renderer, scale_, scale_);
//    if (!renderer)
//    {
//        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//        SDL_DestroyWindow(window);
//        SDL_Quit();
//        return 1;
//    }
//    SDL_Event event;
//---------------------------------------------------------------------------
}

    //MAP
    Map mp("map2.json");
    if (!mp.load())
    {
        std::cout << "Failed to load map!\n";

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    mp.loadTextures(renderer);

    //PLAYER
    Player player;
    player.LoadImg("asset/Characters/generic_char_v0.2/png/blue/CHAR.png", renderer);

    player.SetClips();
    //GAMELOOP
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    bool isRunning = true;
    while (isRunning)
    {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            player.HandleInput(event, renderer);
        }

        SDL_RenderClear(renderer);
        //-----
        mp.render(renderer);
        player.Show(renderer);
        //-----
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    close();
    return 0;
}
