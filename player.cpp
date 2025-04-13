#include "player.h"
#include "map.h"

Player::Player()
{
    x_val = 0;
    y_val = 0;
    frame = 0;
    x_pos = 0;
    y_pos = 64*2;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    status = -1;
    inp_type.left = 0;
    inp_type.right = 0;
    inp_type.up = 0;
    inp_type.down = 0;
    inp_type.jump = 0;
    inp_type.dash = 0;
    on_ground = false;
    mapposx = 0;
    mapposy = 0;
}
bool Player::LoadImg(std::string path, SDL_Renderer* renderer)
{
    bool ret = baseobj::LoadImg(path, renderer);

    if ( ret == true)
    {
        w_frame = rect_.w/2 ;
        h_frame = rect_.h/4;
        w_true = rect_.w/2 - 2 * OffsetX;
        h_true = rect_.h/4 - OffsetY;
    }
    return ret;
}
void Player::SetClips() // cat frame
{
    if (w_frame > 0 && h_frame > 0)
    {
        int index = 0;
        for (int j = 0; j <= 3; ++j) // hàng trước
        {
            for (int i = 0; i <= 1; ++i) // cột sau
                {
                run_clip[index].x = i * w_frame + OffsetX;
                run_clip[index].y = j * h_frame + OffsetY;
                run_clip[index].w = w_true;
                run_clip[index].h = h_true;
                ++index;
            }
        }
    }
}
void Player::UpdateAnimationFrame(int total_frames, ImpTimer& timer, int frame_delay)
{
    if (timer.GetTicks() >= frame_delay)
    {
        frame++;
        if (frame >= total_frames)
        {
            frame = 0;
        }
        timer.Start();
    }
}

void Player::Show(SDL_Renderer* des)
{
    LoadImg("asset/Knight/Run.png", des);

    if (inp_type.left == 1 || inp_type.right == 1)
    {
        UpdateAnimationFrame(7, frame_timer, frame_delay);
    }
    else
    {
        frame = 0;
        frame_timer.Start();
    }

    rect_.x = x_pos - mapposx;
    rect_.y = y_pos - 32 - mapposy ;

    SDL_Rect* current_clip = &run_clip[frame];// cat cho nao
    SDL_Rect renderQuad = {rect_.x, rect_.y , w_true, h_true};
    SDL_RenderCopy(des, p_object_, current_clip, & renderQuad);


    // hien thi hitbox
    SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
    SDL_RenderDrawRect(des, &renderQuad);
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
                inp_type.left = 0;
            }
            break;
            case SDLK_LEFT:
            {
                status= WALK_LEFT;
                inp_type.left = 1;
                inp_type.right = 0;
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
void Player::DoPlayer(MapObject& map_data, MapObject& visual_map)
{
    x_val = 0;
    y_val += 1;

    if ( y_val >= MaxFallSpeed )
        y_val = MaxFallSpeed;


    if ( inp_type.left == 1)
    {
        x_val -= PlayerSpeed;
    }
    else if ( inp_type.right == 1)
    {
        x_val += PlayerSpeed;
    }

    CheckToMap(map_data);
    CenterEntityOnMap(visual_map);
}
void Player::CenterEntityOnMap(MapObject& visual_map)
{

    visual_map.start_x = x_pos - (game_w/2); //cuon theo khi giua map

    if (visual_map.start_x < 0)
    {
        visual_map.start_x = 0;
    }
//    else if (visual_map.start_x + screen_w >=visual_map.max_x)
//    {
//        visual_map.start_x = visual_map.max_x - screen_w;
//    }

    visual_map.start_y = y_pos - (game_h/2);
      if (visual_map.start_y < 0)
    {
        visual_map.start_y = 0;
    }
//    else if (visual_map.start_y + game_h >=visual_map.max_y)
//    {
//        visual_map.start_y = visual_map.max_y - game_h;
//    }
    std :: cout << visual_map.start_x << ' ' << y_pos <<' '<< x_pos <<std :: endl;
}
void Player::CheckToMap(MapObject& map_data)
{
    int x1 = 0;
    int y1 = 0;

    int x2 = 0;
    int y2 = 0;

    int h_min = std::min(h_true, TileSize);

    x1 = (x_pos + x_val )/TileSize;
    x2 = (x_pos + x_val + w_true - 1)/TileSize;

    y1 = (y_pos)/TileSize;
    y2 = (y_pos + h_min - 1)/TileSize;


    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {
        if (x_val > 0) {
        if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                x_pos = x2 * TileSize;
                x_pos -= w_true;
                x_val = 0;
            }
        }
    else
        if (x_val < 0){
        if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos = (x1 + 1) * TileSize ;
                x_val = 0;
            }
        }
    }
    int w_min = w_frame < TileSize ? w_frame : TileSize;
    x1 = (x_pos) / TileSize;
    x2 = (x_pos + w_min) / TileSize;

    y1 = (y_pos + y_val) / TileSize;
    y2 = (y_pos + y_val + h_true - 1) / TileSize;

    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {
        if (y_val > 0)
        {
            if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                y_pos = y2 * TileSize;
                y_pos -= (h_true - 1);
                y_val = 0;
                on_ground = true;
            }
        }
        else if (y_val < 0)
        {
            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
            {
                y_pos = (y2 + 1) * TileSize;
                y_val = 0;
            }
        }
    }
    //std::cout << x1 << ' ' << x2 << ' ' << y1 << ' ' << y2 <<'\n';
    x_pos += x_val;
    y_pos += y_val;
    if ( y_val >= MaxFallSpeed) y_val = MaxFallSpeed;
    if (x_pos < 0)
    {
        x_pos = 0;
    }
    else if ( x_pos + w_frame > 200*32)
    {
        x_pos = 199*32 - w_frame;
    }


}


