#include "map.h"
#include "func.h"
#include "baseobj.h"
#include "player.h"
#include "enemies.h"
#include "timer.h"
#include "menu.h"
#include "submenu.h"


bool inidata() {

    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;
    window = SDL_CreateWindow("SweetyTaste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_w * Scale, game_h * Scale, SDL_WINDOW_SHOWN);
    if (window == NULL) success = false;
    else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) success = false;
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;

        }
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    SDL_RenderSetScale(renderer, Scale, Scale);
    return success;
}
void close() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}
std::string mppath;
int SLEnemy;
EnemyManager enemyManager;
Player player;
Map mp("");
void InitLVL() {
    player.Reset();
    mp = Map(mppath);
    if (!mp.load()) {
        std::cout << "Failed to load map!\n";

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    mp.loadTextures(renderer);
    mp.SaveCollision();
    mp.GetEnemyPos();

    player.LoadImg(renderer);
    player.SetClips();

    enemyManager.Init(renderer, mp.spawn_e, SLEnemy);

}
enum GameState {
    STATE_MENU,
    STATE_PLAY,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_QUIT
};
int main(int argc, char * argv[]) {
    if (!inidata()) return -1;
    ImpTimer Timer;
    GameState state;
    state = STATE_MENU;
    bool isRunning = true;
    while (isRunning) {
        //std :: cout << "state" << state << ' ';
        if (state == STATE_MENU) {
            Menu menu;
            int lv = menu.Show(renderer);
            if (lv == -1) {
                isRunning = false;
            } else {
                if (lv == 1) mppath = "map.json", SLEnemy = 5;
                else if (lv == 2) mppath = "map2.json", SLEnemy = 6;
                else if (lv == 3) mppath = "map3.json",  SLEnemy = 9;

                InitLVL();
                state = STATE_PLAY;
            }
        }
        //----------------------------------------------------
        else if (state == STATE_PLAY) {
            Timer.Start();
            while (SDL_PollEvent( & event)) {
                if (event.type == SDL_QUIT)
                    isRunning = false;
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    state = STATE_PAUSE;
                }

                player.HandleInput(event);
            }

            SDL_RenderClear(renderer);
            mp.render(renderer);
            enemyManager.Update(player, mp.game_map, mp.visual_map, mp.visual_map.start_x, mp.visual_map.start_y, player.GetX(), player.GetY(), SLEnemy);
            enemyManager.Render(renderer, mp.spawn_e, SLEnemy);
            player.SetMapPos(mp.visual_map.start_x, mp.visual_map.start_y);
            player.Show(renderer);
            player.DoPlayer(mp.game_map, mp.visual_map);
            if (player.IsDead() || player.is_win) {
                SDL_Delay(1000);
                state = STATE_GAMEOVER;
            }

            SDL_RenderPresent(renderer);

            int true_time = Timer.GetTicks();
            int time_one_frame = 1000 / fps;
            if (true_time < time_one_frame) SDL_Delay(time_one_frame - true_time);
        }
        //----------------------------------------------------
        else if (state == STATE_GAMEOVER) {
            int choice = -1;
            SubMenu menu;
            menu.is_win = player.is_win;
            menu.showContinue = false;
            choice = menu.Show(renderer);

            std :: cout << menu.showContinue <<'\n';
            if (choice == 1) {
                InitLVL();
                state = STATE_PLAY;
            } else if (choice == 2) {
                state = STATE_MENU;

            }
        }

        //----------------------------------------------------
        else if (state == STATE_PAUSE) {
            SubMenu menu;
            menu.showContinue = true;
            int choice = menu.Show(renderer); // tự viết hàm Show trả về lựa chọn
            if (choice == 3) {
                state = STATE_PLAY;
            } else if (choice == 1) {
                InitLVL();
                state = STATE_PLAY;
            } else if (choice == 2) {
                state = STATE_MENU;
            }
        }
    }
    close();
    return 0;
}
