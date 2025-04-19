#include "player.h"


Player::Player()
{
    x_val = 0;
    y_val = 0;
    frame[0] = frame[1] = frame[2] = frame[3] = frame[4] = frame[5] =0;
    x_pos = 50;
    y_pos = 64*3;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    status = -1;
    inp_type = {0, 0, 0, 0};
    hp = 100;
    on_ground = false;
    mapposx = 0;
    mapposy = 0;
    attack_input_timer.Start();
    is_win = false;
}
void Player::Reset()
{
    x_val = 0;
    y_val = 0;
    frame[0] = frame[1] = frame[2] = frame[3] = frame[4] = frame[5] =0;
    x_pos = 50;
    y_pos = 64*3;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    status = -1;
    inp_type = {0, 0, 0, 0};
    hp = 100;
    on_ground = false;
    mapposx = 0;
    mapposy = 0;
    attack_input_timer.Start();
    is_win = false;
}
bool Player::LoadImg(SDL_Renderer* renderer)
{

    idleTexture = LoadTexture("asset/Knight/Idle.png", renderer);
    runTexture = LoadTexture("asset/Knight/Run.png", renderer);
    jumpTexture = LoadTexture("asset/Knight/Jump.png", renderer);
    attackTexture = LoadTexture("asset/Knight/Attacks.png", renderer);
    tex_bg = LoadTexture("asset/HUD/bar_background.png", renderer);
    tex_hp = LoadTexture("asset/HUD/health_bar.png", renderer);
    tex_bar = LoadTexture("asset/HUD/bar.png", renderer);

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
                frame_clip[index].x = i * w_frame ;
                frame_clip[index].y = j * h_frame + OffsetY;
                frame_clip[index].w = w_frame;
                frame_clip[index].h = h_true;
                ++index;
            }
        }
        index = 0;
        for (int j = 0; j <= 4; ++j) // hàng trước
        {
            for (int i = 0; i <= 7; ++i) // cột sau
                {
                atk_clip[index].x = i * w_frame ;
                atk_clip[index].y = j * h_frame + OffsetY;
                atk_clip[index].w = w_frame ;
                atk_clip[index].h = h_true;
                ++index;
            }
        }
    }
}
void Player::DrawHP(SDL_Renderer* ren, int cur_hp) {
    SDL_Rect dst = {30, 260, 100, 7};
    SDL_RenderCopy(ren, tex_bg, nullptr, &dst);

    dst = {30, 260, cur_hp, 7};
    SDL_RenderCopy(ren, tex_hp, nullptr, &dst);
    dst = {21, 256, 118, 13};
    SDL_RenderCopy(ren, tex_bar, nullptr, &dst);
}

void Player::Show(SDL_Renderer* des)
{
    SDL_Rect* current_clip = nullptr;
    SDL_Texture* currentTexture = nullptr;
    SDL_Rect renderQuad;
//    std :: cout << inp_type.left <<  ' ' << inp_type.right <<  ' ' << inp_type.attack <<  ' ' << inp_type.jump <<  ' '
//    << actionState <<  ' ' << frame[actionState] <<  ' '<< std :: endl;
    rect_.x = x_pos - w_frame/2 - mapposx;
    rect_.y = y_pos - 32 - mapposy ;
    ATK_Rect.x = x_pos - (w_true)* (status == LEFT) - mapposx -3 ;
    ATK_Rect.y = y_pos - mapposy - 32;
    ATK_Rect.w = 40;
    ATK_Rect.h = h_true;


    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (status == LEFT)
    {
        flip = SDL_FLIP_HORIZONTAL;
        direct = 20;
    }
    else
    {
        flip = SDL_FLIP_NONE;
        direct = 0;
    }

    if (actionState != ATTACKING) {

        renderQuad = {rect_.x  , rect_.y , w_frame, h_true};
        if (actionState == JUMPING) {
            currentTexture = jumpTexture;
        }
        else if (actionState == FALLING) {
            currentTexture = jumpTexture;
        }
        else if (actionState == RUNNING) {
            currentTexture = runTexture;
        }
        else  {
            currentTexture = idleTexture;
        }

        current_clip = &frame_clip[frame[actionState]];
        if ( damaged )
        {
            SDL_SetTextureColorMod(currentTexture, 255, 0, 0);
            if ( hurt_timer.GetTicks() >= 200)
                damaged = false;
        }
        else
        {
            SDL_SetTextureColorMod(currentTexture, 255, 255, 255);
            hurt_timer.Start();
        }
        SDL_RenderCopyEx(des, currentTexture, current_clip,& renderQuad, 0.0, nullptr, flip);
    } else
    {
        currentTexture = attackTexture;
        current_clip = &atk_clip[frame[actionState] + direct];
        renderQuad = {rect_.x, rect_.y , w_frame, h_true};
        if ( damaged )
        {
            SDL_SetTextureColorMod(currentTexture, 255, 0, 0);
            if ( hurt_timer.GetTicks() >= 200)
                damaged = false;
        }
        else
        {
            SDL_SetTextureColorMod(currentTexture, 255, 255, 255);
            hurt_timer.Start();
        }
        SDL_RenderCopy(des, currentTexture, current_clip,& renderQuad);
    }
//    SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
//    SDL_RenderDrawRect(des, &ATK_Rect);
//    SDL_SetRenderDrawColor(des, 255, 255, 0, 255);
//    SDL_Rect hitbox = {rect_.x  + w_frame/2 - w_true/2, rect_.y , w_true, h_true};
//    SDL_RenderDrawRect(des, &hitbox);

    DrawHP(des, hp);
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
    if (timer.GetTicks() >= frame_delay)
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
    //std :: cout << frame[actionState] + direct << ' ' << actionState << std :: endl;
    x_val = 0;
    y_val += 1;
    if ( y_val >= MaxFallSpeed )
        y_val = MaxFallSpeed;

    if ((inp_type.left == 1 || inp_type.right == 1) && actionState != ATTACKING)
    {
        actionState = ActionState::RUNNING;
        UpdateRepeatFrame(7, run_timer, frame_delay);
    }
    else
    {
        run_timer.Start();
    }
    if ( inp_type.left == 1 )
    {
        inp_type.right == 0;
        status = LEFT;
        if ( actionState == RUNNING )
        x_val -= PlayerSpeed ;
    }
    else if ( inp_type.right == 1)
    {
        inp_type.left == 0;

        status = RIGHT ;
        if ( actionState == RUNNING )
        x_val += PlayerSpeed ;
    }
    else if ( inp_type.attack == 1)
    {
        actionState = ActionState::ATTACKING;
        attack_input_timer.Start();
        inp_type.attack = 0;
    }
    if ( actionState == ATTACKING)
    {
        if (attack_phase == 0)  UpdateRepeatFrame(20, attack_timer, frame_delay);

        if (attack_input_timer.GetTicks() >= 600)
        {
            actionState = ActionState::IDLE;
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
        UpdateNoRepeatFrame(3, jump_timer, 5);
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
    if (inp_type.left != 1 && inp_type.right != 1 &&  actionState != JUMPING && actionState != ATTACKING)
    {
        frame[3] = 0;
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
            case SDLK_j:
                inp_type.attack = 1; break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_a: inp_type.left = 0; break;
            case SDLK_d: inp_type.right = 0; break;
            case SDLK_j: inp_type.attack = 0; break;
        }
    }

}
void Player::CenterEntityOnMap(MapObject& visual_map)
{

    visual_map.start_x = x_pos - (game_w/2);

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
    else if (visual_map.start_y + game_h >=17 * TileSize)
   {
        visual_map.start_y = 17 * TileSize - game_h;
   }
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

    x1 = (x_pos + x_val - w_true/2 )/TileSize;
    x2 = (x_pos + x_val + w_true/2 - 1)/TileSize;

    y1 = (y_pos)/TileSize;
    y2 = (y_pos + h_min - 1)/TileSize;


    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {
        if (x_val > 0) {
        if (( map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != SLOPE_TILE_DOWN)
            || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != SLOPE_TILE_DOWN))
            {
                x_pos = x2 * TileSize;
                x_pos -= w_true/2;
                x_val = 0;
            }
        }
    else
        if (x_val < 0){
        if (( map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x2] != SLOPE_TILE_DOWN)
            || (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x2] != SLOPE_TILE_DOWN))
            {
                x_pos = (x1 + 1) * TileSize +1 + w_true/2;
                x_val = 0;
            }
        }

    }

    // Y
    //std :: cout << hp <<  '\n';
    int w_min = w_true < TileSize ? w_true : TileSize;
    x1 = (x_pos - w_true/2 + 9 ) / TileSize;
    x2 = (x_pos + w_true/2 - 9) / TileSize;

    y1 = (y_pos + y_val) / TileSize;
    y2 = (y_pos + y_val + h_true - 1) / TileSize;

    int px = x_pos;
    int tile_x = px / TileSize;
    int tile_y = y2;

    if (x1 >= 0 && x2 < game_w && y1 >= 0 && y2 < game_h)
    {
        if ( map_data.tile[y2][x1] == 141 || map_data.tile[y2][x2] == 141 )
        {
            if (!first_strike) hp-=10, first_strike = 1, damaged = true;
            if ( spike_timer.GetTicks() >= 1000)
               hp -= 5, spike_timer.Start() , damaged = true;
        }
        else spike_timer.Start() , first_strike = 0;
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
    if ( x2 == 199) is_win = true;
    if ( y_val >= MaxFallSpeed) y_val = MaxFallSpeed;
    if (x_pos - w_true/2 < 0)
    {
        x_pos  = w_true/2 +1;
    }
    else if ( x_pos + w_true/2 > MapX * TileSize)
    {
        x_pos = MapX * TileSize - w_true/2;
    }
    if ( y2 >= MapY) hp-=2;

}


