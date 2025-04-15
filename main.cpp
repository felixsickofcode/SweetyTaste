#include "map.h"
#include "func.h"
#include "baseobj.h"
#include "player.h"
#include "timer.h"

bool inidata()
{

    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if ( ret < 0) return false;
    window = SDL_CreateWindow("SweetyTaste",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_w * Scale, game_h * Scale, SDL_WINDOW_SHOWN);
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
    SDL_RenderSetScale(renderer, Scale, Scale);
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
    ImpTimer Timer;
    if (!inidata()) return -1;
    //MAP
    Map mp("map.json");
    if (!mp.load())
    {
        std::cout << "Failed to load map!\n";

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    mp.loadTextures(renderer);
    mp.SaveCollision();
    //PLAYER
    Player player;
    player.LoadImg(renderer);
    player.SetClips();
    //GAMELOOP

    bool isRunning = true;
    while (isRunning)
    {
        Timer.Start();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            player.HandleInput(event);
        }

        SDL_RenderClear(renderer);
        //-----

        mp.render(renderer);
        player.Show(renderer);
        player.SetMapPos(mp.visual_map.start_x, mp.visual_map.start_y);
        player.DoPlayer(mp.game_map, mp.visual_map);

        //-----
        SDL_RenderPresent(renderer);

        int true_time = Timer.GetTicks();
        int time_one_frame = 1000 / fps;
        if (true_time < time_one_frame)
        {
            int delay_time = time_one_frame - true_time;
            if ( delay_time >= 0)
                SDL_Delay(delay_time);

        }

    }

    close();
    return 0;
}
