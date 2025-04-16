#include "player.h"


Player::Player()
{
    x_val = 0;
    y_val = 0;
    frame[0] = frame[1] = frame[2] = frame[3] = frame[4] = frame[5] =0;
    x_pos = 0;
    y_pos = 64*3;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    status = -1;
    inp_type = {0, 0, 0, 0};
    on_ground = false;
    mapposx = 0;
    mapposy = 0;
    jump_input_timer.Stop();
}
bool Player::LoadImg(SDL_Renderer* renderer)
{

    idleTexture = LoadTexture("asset/Knight/Idle.png", renderer);
    runTexture = LoadTexture("asset/Knight/Run.png", renderer);
    jumpTexture = LoadTexture("asset/Knight/Jump.png", renderer);
    attackTexture = LoadTexture("asset/Knight/Attacks.png", renderer);

    bool ret = baseobj::LoadImg("asset/Knight/Run.png", renderer);

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
                frame_clip[index].x = i * w_frame + OffsetX;
                frame_clip[index].y = j * h_frame + OffsetY;
                frame_clip[index].w = w_true;
                frame_clip[index].h = h_true;
                ++index;
            }
        }
        index = 0;
        for (int j = 0; j <= 4; ++j) // hàng trước
        {
            for (int i = 0; i <= 7; ++i) // cột sau
                {
                atk_clip[index].x = i * w_frame + 40 ;
                atk_clip[index].y = j * h_frame + OffsetY;
                atk_clip[index].w = w_frame - 60;
                atk_clip[index].h = h_true;
                ++index;
            }
        }
    }
}

void Player::Show(SDL_Renderer* des)
{
//    std :: cout << inp_type.left <<  ' ' << inp_type.right <<  ' ' << inp_type.attack <<  ' ' << inp_type.jump <<  ' '
//    << actionState <<  ' ' << frame[actionState] <<  ' '<< std :: endl;
    rect_.x = x_pos - mapposx;
    rect_.y = y_pos - 32 - mapposy ;
    if (actionState == ATTACKING) {
        w_hitbox = w_frame - 60;
        h_hitbox = h_true;
    }
    else
    {
        w_hitbox = w_true;
        h_hitbox = h_true;
    }

    SDL_Rect renderQuad = {rect_.x, rect_.y , w_hitbox, h_hitbox};

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (status == LEFT)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        flip = SDL_FLIP_NONE;
    }

    SDL_Rect* current_clip = nullptr;
    SDL_Texture* currentTexture = nullptr;
    if (actionState != ATTACKING) {
        if (actionState == JUMPING) {
            currentTexture = jumpTexture;
        }
        else if (actionState == FALLING) {
            currentTexture = jumpTexture;
        }
        else if (actionState == RUNNING) {
            currentTexture = runTexture;
        }
        else  currentTexture = idleTexture;
        current_clip = &frame_clip[frame[actionState]];
    } else
    {
        current_clip = &atk_clip[frame[actionState]];
        currentTexture = attackTexture;
    }

    SDL_RenderCopyEx(des, currentTexture, current_clip,& renderQuad, 0.0, nullptr, flip);

    SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
    SDL_RenderDrawRect(des, &renderQuad);
}
void Player::UpdateRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay)
{

    if (timer.GetTicks() >= frame_delay)
    {

        frame[actionState]++;
        if (frame[actionState] >= total_frames)
        {
            frame[actionState] = 0 ;
        }
        timer.Start();
    }
}

void Player::UpdateNoRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay)
{
    if (timer.GetTicks() >= 5)
    {
        frame[actionState]++;
        if (frame[actionState] >= total_frames)
        {
            frame[actionState] = total_frames  ;
        }
        timer.Start();
    }
}

void Player::DoPlayer(MapObject& map_data, MapObject& visual_map)
{

    //std :: cout << frame[actionState] << ' ' << actionState << std :: endl;
    x_val = 0;
    y_val += 1;
    if ( y_val >= MaxFallSpeed )
        y_val = MaxFallSpeed;

    if ((inp_type.left == 1 || inp_type.right == 1) &&  inp_type.jump == 0 && inp_type.attack == 0)
    {
        actionState = ActionState::RUNNING;
        UpdateRepeatFrame(7, run_timer, frame_delay);
    }
    else
    {
        run_timer.Start();
    }
    if ( inp_type.left == 1 && inp_type.attack == 0 )
    {
        status = LEFT;
        x_val -= PlayerSpeed;
    }
    if ( inp_type.right == 1 &&  inp_type.attack == 0)
    {
        status = RIGHT;
        x_val += PlayerSpeed;
    }
    else if ( inp_type.attack == 1)
    {
        actionState = ActionState::ATTACKING;
        UpdateRepeatFrame(7, attack_timer, frame_delay);
        if ( attack_input_timer.GetTicks() >= 700)
        {
            inp_type.attack = 0;
        }
    }
    else
    {
        attack_timer.Start();
    }

    if ( inp_type.jump == 1 )
    {
        frame[3] = 0;
        y_val = - PlayerJump;
        actionState = ActionState::JUMPING;
        UpdateNoRepeatFrame(3, jump_timer, frame_delay);
        if ( jump_input_timer.GetTicks() >= 300)
        {
            inp_type.jump = 0;
        }
    }
    else
    {
        jump_timer.Start();
    }

    if ( (inp_type.jump == 0 ) && on_ground == 0)
    {

        actionState = ActionState::FALLING;
        frame[actionState] = 4;
        UpdateNoRepeatFrame(3, jump_timer, frame_delay);
    }
    else
    {
        jump_timer.Start();
    }
    if (inp_type.left != 1 && inp_type.right != 1 &&  inp_type.jump != 1 && inp_type.attack != 1)
    {
        if (actionState != ActionState::IDLE) {
        idle_timer.Start();
        actionState = ActionState::IDLE;
    }
        UpdateRepeatFrame(7, idle_timer, frame_delay);
    }
    else idle_timer.Start();

    //std :: cout << actionState << std :: endl;
    CheckToMap(map_data);
    CenterEntityOnMap(visual_map);
}
void Player::HandleInput(SDL_Event event)
{

    if ( init == 0)
    {
        inp_type = {0, 0, 0, 0};

        init = 1;
    }
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_a: inp_type.left = 1; break;
            case SDLK_d: inp_type.right = 1; break;
            case SDLK_SPACE: if ( on_ground == 1)
                inp_type.jump = 1, jump_input_timer.Start(); break;
            case SDLK_u:
                inp_type.attack = 1, attack_input_timer.Start(); break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_a: inp_type.left = 0; break;
            case SDLK_d: inp_type.right = 0; break;
            //case SDLK_u: frame[3] = 0; break;
        }
    }

}
void Player::CenterEntityOnMap(MapObject& visual_map)
{

    visual_map.start_x = x_pos - (game_w/2); //cuon theo khi giua map

    if (visual_map.start_x < 0)
    {
        visual_map.start_x = 0;
    }
    else if (visual_map.start_x + game_w >= MapX * TileSize)
    {
        visual_map.start_x = MapX * TileSize - game_w;
    }

    visual_map.start_y = y_pos - (game_h/2);
      if (visual_map.start_y < 0)
    {
        visual_map.start_y = 0;
    }
//    else if (visual_map.start_y + game_h >=visual_map.max_y)
//    {
//        visual_map.start_y = visual_map.max_y - game_h;
//    }
    //std :: cout << visual_map.start_x << ' ' << y_pos <<' '<< x_pos <<std :: endl;
}

int getYOnSlopeDown(int tile_x, int tile_y, int px) {
    int x0 = tile_x * TileSize ;
    int y0 = tile_y * TileSize ;
    int dx = px - x0;
    if (dx < 0) dx = 0;
    if (dx >= TileSize) dx = TileSize - 1;
    return y0 + dx;
}
void Player::CheckToMap(MapObject& map_data)
{
    on_ground = false;

    int x1 = 0;
    int y1 = 0;

    int x2 = 0;
    int y2 = 0;



    // X

    int h_min = std::min(h_true, TileSize);

    x1 = (x_pos + x_val )/TileSize;
    x2 = (x_pos + x_val + w_true - 1)/TileSize;

    y1 = (y_pos)/TileSize;
    y2 = (y_pos + h_min - 1)/TileSize;


    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {
        if (x_val > 0) {
        if (( map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != SLOPE_TILE_DOWN)
            || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != SLOPE_TILE_DOWN))
            {
                x_pos = x2 * TileSize;
                x_pos -= w_true;
                x_val = 0;
            }
        }
    else
        if (x_val < 0){
        if (( map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x2] != SLOPE_TILE_DOWN)
            || (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x2] != SLOPE_TILE_DOWN))
            {
                x_pos = (x1 + 1) * TileSize +1 ;
                x_val = 0;
            }
        }

    }

    // Y

    int w_min = w_frame < TileSize ? w_frame : TileSize;

    x1 = (x_pos) / TileSize;
    x2 = (x_pos + w_min) / TileSize;

    y1 = (y_pos + y_val) / TileSize;
    y2 = (y_pos + y_val + h_true - 1) / TileSize;

    int px = x_pos + w_true / 2;
    int tile_x = px / TileSize;
    int tile_y = y2;

    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {

        if (y_val > 0)
        {

            //std :: cout << tile_x << ' ' << tile_y << std::endl;
            if (map_data.tile[tile_y][tile_x] == SLOPE_TILE_DOWN)
            {
                int y_slope = getYOnSlopeDown(tile_x, tile_y, px);
                y_pos = y_slope - h_true - 2;
                y_val = 0;
                on_ground = true;

            }


            else
            if ((map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != SLOPE_TILE_DOWN)
                || ( map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != SLOPE_TILE_DOWN))
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
                y_pos = (y1 + 1) * TileSize;
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
    else if ( x_pos + w_true > MapX * TileSize)
    {
        x_pos = MapX * TileSize - 1 - w_true;
    }
}


