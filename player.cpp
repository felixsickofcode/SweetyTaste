#include "player.h"

Player::Player()
{
    x_val = 0;
    y_val = 0;
    frame = 0;
    x_pos = 0;
    y_pos = 0;
    w_frame = 0;
    h_frame = 0;
    status = -1;
}
bool Player::LoadImg(std::string path, SDL_Renderer* renderer)
{
    bool ret = baseobj::LoadImg(path, renderer);

    if ( ret == true)
    {
        w_frame = rect_.w/8;
        h_frame = rect_.h/11;
    }
    return ret;
}
void Player::SetClips()
{
    if (w_frame > 0 && h_frame > 0)
    {
        for (int i = 0; i <= 5; i++)
        {
            frame_clip[i].x = i * w_frame;
            frame_clip[i].y = 2 * h_frame;
            frame_clip[i].w = w_frame;
            frame_clip[i].h = h_frame;
        }
    }

}
void Player::Show(SDL_Renderer* des)
{
    LoadImg("asset/Characters/generic_char_v0.2/png/blue/CHAR.png", des);

    if(inp_type.left == 1 || inp_type.right == 1)
        frame++;
    else
        frame = 0;
    if (frame >= 6)
        frame = 0;
    rect_.x = x_pos;
    rect_.y = y_pos;

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect_.x, rect_.y,w_frame, h_frame};
    SDL_RenderCopy(des, p_object_, current_clip, & renderQuad);
}

void Player::HandleInput(SDL_Event events, SDL_Renderer* screen)
{
    if ( events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_RIGHT:
            {
                status= WALK_RIGHT;
                inp_type.right = 1;
            }
            break;
            case SDLK_LEFT:
            {
                status= WALK_LEFT;
                inp_type.left = 1;
            }
            break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_RIGHT:
            {
                status= WALK_RIGHT;
                inp_type.right = 0;
            }
            break;
            case SDLK_LEFT:
            {
                status= WALK_LEFT;
                inp_type.left = 0;
            }
            break;
        }
    }


}
