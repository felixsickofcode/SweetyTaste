#include "timer.h"

ImpTimer::ImpTimer()
{
    start_tick = 0;
    pause_tick = 0;
    is_paused = false;
    is_started = false;
}
ImpTimer::~ImpTimer()
{
}

void ImpTimer::Start()
{
    is_started = true;
    is_paused = false;
    start_tick = SDL_GetTicks();
}

void ImpTimer::Stop()
{
    is_started = false;
    is_paused = false;
}

void ImpTimer::Pause()
{
    if (is_started == true && is_paused == false)
    {
        is_paused = true;
        pause_tick = SDL_GetTicks() - start_tick;
    }
}

void ImpTimer::UnPause()
{
    if (is_paused == true)
    {
        is_paused = false;
        start_tick = SDL_GetTicks() - pause_tick;
        pause_tick = 0;
    }
}


int ImpTimer::GetTicks()
{
    if (is_started == true)
    {
        if (is_paused == true)
        {
            return pause_tick;
        }
        else
        {
            return SDL_GetTicks() - start_tick;
        }
    }
}

bool ImpTimer::IsStarted()
{
    return is_started;
}

bool ImpTimer::IsPaused()
{
    return is_paused;
}



