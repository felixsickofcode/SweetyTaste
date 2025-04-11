#ifndef PLAYER_H
#define PLAYER_H

#include "baseobj.h"
#include "func.h"

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
    void Show(SDL_Renderer* des);
    void HandleInput(SDL_Event events, SDL_Renderer* screen);
    void SetClips();
private:

        float x_val;
        float y_val;

        float x_pos;
        float y_pos;

        int w_frame;
        int h_frame;

        SDL_Rect frame_clip[8];
        Input inp_type;
        int frame;
        int status;
};

#endif
