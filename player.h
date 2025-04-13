#ifndef PLAYER_H
#define PLAYER_H

#include "baseobj.h"
#include "func.h"
#include "timer.h"

class Player : public baseobj
{
public:
    Player();
    ~Player() = default;

    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
    };
    bool LoadImg(std::string path, SDL_Renderer* screen);

    void UpdateAnimationFrame(int total_frames, ImpTimer& timer, int frame_delay);
    void Show(SDL_Renderer* des);
    void HandleInput(SDL_Event events, SDL_Renderer* screen);
    void SetClips();
    void CheckToMap(MapObject& map_data);
    void CenterEntityOnMap(MapObject& visual_map);
    void DoPlayer(MapObject& map_data, MapObject& visual_map);
    void SetMapPos(const int x, const int y)
    {
        mapposx = x;
        mapposy = y;
    }
private:

        float x_val;
        float y_val;

        float x_pos;
        float y_pos;

        int w_frame;
        int h_frame;
        int w_true;
        int h_true;

        SDL_Rect run_clip[8];
        Input inp_type;
        int frame;
        int status;
        bool on_ground;

        int mapposx;
        int mapposy;
        ImpTimer frame_timer;
        int frame_delay = 100;
};

#endif
