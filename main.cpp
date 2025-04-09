#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "map.h"

int main(int argc, char* argv[])
{
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("SweetyTaste",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          2 * 320*2, 32*2*11,
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(renderer, 2, 2);
    if (!renderer)
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Map mp("map2.json");

    if (!mp.load())
    {
        std::cout << "Failed to load map!\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //mp.loadTilesetsInfo();
    mp.loadTextures(renderer);

    bool isRunning = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (isRunning)
    {
        frameStart = SDL_GetTicks();

        // Xử lý sự kiện
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        // Update game logic (nếu có)
        // TODO: sau này thêm cập nhật nhân vật, camera, animation tại đây

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue background
        SDL_RenderClear(renderer);

        // Render map
        mp.render(renderer);

        // Present renderer
        SDL_RenderPresent(renderer);

        // Giới hạn FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Dọn dẹp
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
