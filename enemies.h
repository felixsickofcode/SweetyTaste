#ifndef ENEMIES_H
#define ENEMIES_H

#include "baseobj.h"
#include "func.h"
#include "timer.h"
#include "enemies.h"
#include "player.h"

#define OffsetY 25
#define OffsetX 47
#define MaxFallSpeed 4
#define PlayerSpeed 0.5
#define PlayerJump 2.5

class Enemy : public baseobj
{
public:
    Enemy();
    ~Enemy() = default;
    void Reset();
    enum Direction
    {
        RIGHT = 1,
        LEFT = -1,
    };
    enum ActionState
    {
        IDLE,
        RUNNING,
        ATTACKING,
        HURT,
        DIE,
    };

    bool LoadImg( SDL_Renderer* screen);
    void Audio(AudioManager& audio);
    void DealDamage(Player& player);
    void GetDamage(Player& player);
    void UpdateRepeatFrame(int total_frames, ImpTimer& timer, int frame_delay);
    void Die(int total_frames, ImpTimer& timer, int frame_delay);
    void Show(SDL_Renderer* des, EnemySpawnPoint spawn_e[]);
    void SetClips();
    void MoveToPlayer(float player_x, float player_y);
    void CheckToMap(MapObject& map_data);
    void DoPlayer(MapObject& map_data, MapObject& visual_map, float player_x, float player_y, Player& player);
    void SetMapPos(const int x, const int y)
    {
        mapposx = x;
        mapposy = y;
    }
    void SetPos(float x, float y) {
        x_pos = x;
        y_pos = y;
    }

    bool spawned = 0;
    int e_hp;
private:
    int lastAttackFrame = -1;

    bool playedDieSound = false;

    bool is_knockback = false;
    int knockback_distance = 30;
    int knockback_speed = 4;
    int knockback_dir = 0;

    SDL_Rect HITBOX;
    int adjusthitbox;

    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* runTexture = nullptr;
    SDL_Texture* jumpTexture = nullptr;
    SDL_Texture* attackTexture = nullptr;
    SDL_Texture* hurtTexture = nullptr;
    SDL_Texture* dieTexture = nullptr;
    bool hasTakenDamage = false;

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
    int mapposx;
    int mapposy;

    SDL_Rect frame_clip[9];
    SDL_Rect atk_clip[33];
    SDL_Rect die_clip[33];

    Input inp_type;

    int frame[10];
    ActionState actionState;
    int status;
    bool on_ground;
    bool can_attack;

        ImpTimer run_timer;
        ImpTimer next_attack_timer;
        ImpTimer current_attack_timer;
        ImpTimer idle_timer;
        ImpTimer attack_timer;
        ImpTimer die_timer;
        ImpTimer hurt_timer;
        int frame_delay = 100;
};
class EnemyManager {
private:
    Enemy e[10];

public:
    Enemy& GetEnemy(int i) { return e[i]; }

    void Init(SDL_Renderer* renderer, EnemySpawnPoint sp[], int SL);
    void Update(Player& player, MapObject& map_data, MapObject& visual_map, int X, int Y, float playerX, float playerY, int SL, AudioManager& audio);
    void Render(SDL_Renderer* renderer, EnemySpawnPoint sp[], int SL);
};







#endif

