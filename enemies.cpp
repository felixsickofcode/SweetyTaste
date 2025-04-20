
#include "enemies.h"

Enemy::Enemy()
{
    hasTakenDamage = false;
    x_val = 0;
    y_val = 0;
    frame[0] = frame[1] = frame[2] = frame[3] = frame[4] = frame[5] =0;
    x_pos = 0;
    y_pos = 64*2;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    e_hp = 39;
    on_ground = false;
    spawned = false;
    can_attack = true;
    next_attack_timer.Start();
    current_attack_timer.Start();
    idle_timer.Start();
}
void Enemy::Reset()
{
    hasTakenDamage = false;
    x_val = 0;
    y_val = 0;
    frame[0] = frame[1] = frame[2] = frame[3] = frame[4] = frame[5] =0;
    x_pos = 0;
    y_pos = 64*2;
    w_frame = 0;
    h_frame = 0;
    w_true = 0;
    h_true = 0;
    e_hp = 39;
    on_ground = false;
    spawned = false;
    can_attack = true;
    next_attack_timer.Start();
    current_attack_timer.Start();
    idle_timer.Start();
}
bool Enemy::LoadImg(SDL_Renderer* renderer)
{

    idleTexture = LoadTexture("asset/Sekeleton/⁯Idle.png", renderer);
    runTexture = LoadTexture("asset/Sekeleton/Run.png", renderer);
    attackTexture = LoadTexture("asset/Sekeleton/Attack.png", renderer);
    hurtTexture = LoadTexture("asset/Sekeleton/Hurt.png", renderer);
    dieTexture = LoadTexture("asset/Sekeleton/Die.png", renderer);


    bool ret = baseobj::LoadImg("asset/Sekeleton/Run.png", renderer);

    if ( ret == true)
    {
        w_frame = rect_.w/10 ;
        h_frame = rect_.h;
        w_true = rect_.w/10;
        h_true = rect_.h - OffsetY;
    }
    return ret;
}
void Enemy::SetClips() // cat frame
{
    //std:: cout << w_frame <<' ';
    if (w_frame > 0 && h_frame > 0)
    {
        int index = 0;
        for (int i = 0; i <= 7; ++i)
        {
            frame_clip[index].x = i * w_frame ;
            frame_clip[index].y = OffsetY;
            frame_clip[index].w = w_true;
            frame_clip[index].h = h_true ;
            ++index;
        }
        index = 0;
        for (int j = 0; j <= 4; ++j) // hàng trước
        {
            for (int i = 0; i <= 4; ++i) // cột sau
            {
                atk_clip[index].x = i * 146 + 16 ;
                atk_clip[index].y = j * h_frame + OffsetY - 10;
                atk_clip[index].w = 100 ;
                atk_clip[index].h = h_true +10;

                die_clip[index].x = i * 118;
                die_clip[index].y = j * 64;
                die_clip[index].w = 118 ;
                die_clip[index].h = 64;

                ++index;
            }
        }
    }
}

void Enemy::Show(SDL_Renderer* des, EnemySpawnPoint spawn_e[])
{

    rect_.x = x_pos - mapposx;
    rect_.y = y_pos - 32 - mapposy ;
    if (actionState == ATTACKING) {
        w_hitbox = 100;
        h_hitbox = h_true +10;
    }
    else
    {
        w_hitbox = w_true;
        h_hitbox = h_true ;
    }
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (status == RIGHT)
    {
        flip = SDL_FLIP_HORIZONTAL;
        adjusthitbox = 0;
    }
    else
    {
        flip = SDL_FLIP_NONE;
        if ( actionState == ATTACKING) adjusthitbox = 30;
    }
    HITBOX = {rect_.x + adjusthitbox, rect_.y , w_true, h_true};
    SDL_Rect renderQuad = {rect_.x, rect_.y , w_hitbox, h_hitbox};
    SDL_Rect* current_clip = &frame_clip[frame[actionState]];
    SDL_Texture* currentTexture = nullptr;
    if ( e_hp <= 0)
    {
        current_clip = &die_clip[frame[actionState]];
        renderQuad = {rect_.x, rect_.y - 15 , 118, 64};
        currentTexture = dieTexture;
        SDL_RenderCopyEx(des, currentTexture, current_clip,& renderQuad, 0.0, nullptr, flip);
    }
    else {
        if (actionState != ATTACKING) {

            if (actionState == RUNNING) {
                currentTexture = runTexture;
            }
            else
            {
                currentTexture = idleTexture;
                current_clip = &frame_clip[frame[actionState]];
            }
        }
        else
        {

            current_clip = &atk_clip[frame[actionState]];
            currentTexture = attackTexture;


        }
        //std :: cout << frame[actionState] << '\n';
        if ( hasTakenDamage )
        {
            SDL_SetTextureColorMod(currentTexture, 255, 0, 0);
        }
        else
        {
            SDL_SetTextureColorMod(currentTexture, 255, 255, 255);
        }
        SDL_RenderCopyEx(des, currentTexture, current_clip,& renderQuad, 0.0, nullptr, flip);
    }
    //SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
    //SDL_RenderDrawRect(des, &renderQuad);
    //SDL_SetRenderDrawColor(des, 255, 0, 255, 255);
    //SDL_RenderDrawRect(des, &HITBOX);
}
void Enemy::MoveToPlayer(float player_x, float player_y) {
    if (x_pos + 50 < player_x + 15) {
        status = RIGHT;
        knockback_dir = -1;
    }
    else if (x_pos + 50 > player_x + 15) {
        status = LEFT;
        knockback_dir = 1;
    }
    else x_val = 0;
    actionState = RUNNING;
}
void Enemy::UpdateRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay)
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
void Enemy::Die(int total_frames, ImpTimer& timer, int frame_delay)
{
    if (timer.GetTicks() >= frame_delay)
    {
        frame[actionState]++;
        if (frame[actionState] >= total_frames)
        {
            frame[actionState] = total_frames  ;
            spawned = true;

        }
        timer.Start();
    }
}
 void Enemy::DealDamage(Player& player)
 {
     if ( actionState == ATTACKING && frame[actionState] == 4 && !spawned)
     {
         player.hp -= 1;
         player.damaged = true;
     }

 }
void Enemy::GetDamage(Player& player)
{
    if (player.GetActionState() == 3 && ( player.GetFrame() == 4 ||player.GetFrame() == 8 ||  player.GetFrame() == 12 ||  player.GetFrame() == 18 ))
    {
        if (!hasTakenDamage)
        {
            e_hp -= 10;
            hasTakenDamage = true;
            hurt_timer.Start();
            is_knockback = true;
        }
    }
    else
    {
        hasTakenDamage = false;
    }
}
void Enemy::Audio(AudioManager& audio)
{
    if (actionState == DIE && !playedDieSound) {
        audio.playSound("edie");
        playedDieSound = true;
    }

    if (actionState != DIE) {
        playedDieSound = false;
    }
}

void Enemy::DoPlayer(MapObject& map_data, MapObject& visual_map, float player_x, float player_y, Player& player)
{

    if (e_hp <= 0)
    {
        actionState = DIE;
        Die(22, die_timer, frame_delay);
        return;
    }
    else die_timer.Start();
    if (is_knockback)
    {
        x_pos += knockback_dir * knockback_speed;
        knockback_distance -= knockback_speed;
        if (knockback_distance <= 0) {
            is_knockback = false;
            knockback_distance = 20;
        }
        return;
    }

    if ( CheckCollision(HITBOX, player.GetATKRect())) GetDamage(player);
    else hasTakenDamage = 0;
    y_val += 1;

    float dx = x_pos - player_x +35;
    float dy = y_pos - player_y ;
    float distance = sqrt(dx * dx + dy * dy);

    if ( y_val >= MaxFallSpeed ) y_val = MaxFallSpeed;
    MoveToPlayer( player_x, player_y);

    if ( distance < 40 )  actionState = ActionState::ATTACKING, x_val = 0 ;

    else if ( distance < 200) x_val = PlayerSpeed * (status) ;
    else
    {
        actionState = ActionState::IDLE;
        x_val = 0;
    }
    if ( actionState == ActionState::RUNNING)
    {
        x_pos += x_val;
        UpdateRepeatFrame(7, run_timer, frame_delay);
    }
    else
    {
        run_timer.Start();
    }

    if ( actionState == ActionState::IDLE)
    {
        UpdateRepeatFrame(7, idle_timer, frame_delay);
    }
    else
    {
        idle_timer.Start();
    }

    if ( actionState == ActionState::HURT   )
    {
        UpdateRepeatFrame(2, hurt_timer, frame_delay);
    }
    else
    {
        hurt_timer.Start();
    }

    if (next_attack_timer.GetTicks() >= 1000) {
        current_attack_timer.Start();
        can_attack = true;
    }

    if (actionState == ActionState::ATTACKING && can_attack) {
        next_attack_timer.Start();
        UpdateRepeatFrame(10, attack_timer, 100);
        if (current_attack_timer.GetTicks() >= 1000)
            can_attack = false, frame[2] = 0;
    }
    else
    {
        attack_timer.Start();
    }
    CheckToMap(map_data);
}

void Enemy::CheckToMap(MapObject& map_data)
{

    on_ground = false;

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
        if ( map_data.tile[y1][x2] != BLANK_TILE
            || map_data.tile[y2][x2] != BLANK_TILE )
            {
                x_pos = x2 * TileSize;
                x_pos -= w_true;
                x_val = 0;
            }
        }
    else
        if (x_val < 0){
        if ( map_data.tile[y1][x1] != BLANK_TILE
            || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos = (x1 + 1) * TileSize +1 ;
                x_val = 0;
            }
        }

    }
    //std::cout << x1 <<' ' << x2 <<  map_data.tile[y1][x1]  <<std:: endl;
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
            if (map_data.tile[y2][x1] != BLANK_TILE
                ||  map_data.tile[y2][x2] != BLANK_TILE )
            {
                y_pos = y2 * TileSize -h_true ;
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



void EnemyManager::Init(SDL_Renderer* renderer, EnemySpawnPoint spawn_e[], int SL) {
    for (int i  =0; i < SL; i++) {
        e[i].Reset();
        e[i].LoadImg(renderer);
        e[i].SetClips();
        e[i].SetPos( spawn_e[i].posx -30 , spawn_e[i].posy - 32);
    }
}

void EnemyManager::Update(Player& player, MapObject& map_data, MapObject& visual_map, int X, int Y, float playerX, float playerY, int SL, AudioManager& audio) {
    for (int i  = 0; i  < SL; i++) {

        if ( !e[i].spawned ) e[i].DoPlayer(map_data, visual_map, playerX, playerY, player);
        e[i].SetMapPos(X, Y);
        e[i].DealDamage(player);
        e[i].Audio(audio);
    }
}

void EnemyManager::Render(SDL_Renderer* renderer, EnemySpawnPoint sp[], int SL) {
    for (int i  = 0; i  < SL; i++) {
        if ( !e[i].spawned ) e[i].Show(renderer, sp);
    }
}

