#ifndef PLAYER_H
#define PLAYER_H

#include "baseobj.h"
#include "func.h"
#include "timer.h"

#define OffsetY 17
#define OffsetX 47
#define MaxFallSpeed 4
#define PlayerSpeed 4.5
#define PlayerJump 2.5
class Player : public baseobj
{
public:
    Player();
    ~Player() = default;

    enum Direction
    {
        RIGHT = 0,
        LEFT = 1,
    };
    enum ActionState
    {
        IDLE,
        RUNNING,
        JUMPING,
        ATTACKING,
        FALLING,
    };
    bool LoadImg( SDL_Renderer* screen);
    void UpdateRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay);
    void UpdateNoRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay);
    void Show(SDL_Renderer* des);
    void HandleInput(SDL_Event event);
    void SetClips();
    void CheckToMap(MapObject& map_data);
    void CenterEntityOnMap(MapObject& visual_map);
    void DoPlayer(MapObject& map_data, MapObject& visual_map);
    void SetMapPos(const int x, const int y)
    {
        mapposx = x;
        mapposy = y;
    }
    float GetX() const { return x_pos; }
    float GetY() const { return y_pos; }

private:
    bool init = 0;
    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* runTexture = nullptr;
    SDL_Texture* jumpTexture = nullptr;
    SDL_Texture* attackTexture = nullptr;

    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int w_frame;
    int h_frame;
    int w_true;
    int h_true;
    int w_hitbox;
    int h_hitbox;

    SDL_Rect frame_clip[9];
    SDL_Rect atk_clip[33];

    Input inp_type;
    ActionState actionState;

    int frame[10];

    int status;
    bool on_ground;

    int mapposx;
    int mapposy;

    ImpTimer run_timer;
    ImpTimer jump_timer;
    ImpTimer idle_timer;
    ImpTimer attack_timer;
    ImpTimer jump_input_timer;
    ImpTimer attack_input_timer;
    int frame_delay = 100;
};

#endif
