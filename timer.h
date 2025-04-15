#ifndef TIMER_H
#define TIMER_H

#include "func.h"

class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();

    void Start();
    void Stop();
    void Pause();
    void UnPause();

    int GetTicks();

    bool IsStarted();
    bool IsPaused();

    int start_tick;
private:

    int pause_tick;

    bool is_paused;
    bool is_started;
};

#endif // TIMER_H_
