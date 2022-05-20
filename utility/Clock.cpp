//
// Created by Tom on 19/05/2022.
//

#include "Clock.h"

void Clock::restart()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    _start_time = getMicros(t) ;

    paused = false;
}

uint64_t Clock::getMicros(timeval t)
{
    return ((long)(t.tv_usec)) + (((long)(t.tv_sec))*1000000ll);
}

void Clock::pause()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    _paused_time = getMicros(t);

    paused = true;
}

void Clock::resume()
{
    paused = false;

    struct timeval t;
    gettimeofday(&t, NULL);
    uint64_t pause = getMicros(t) - _paused_time;

    _start_time += pause;
}

uint64_t Clock::elapsed_us()
{
    if (paused) {
        return _paused_time - _start_time;
    } else {
        struct timeval t;
        gettimeofday(&t, NULL);
        return getMicros(t) - _start_time;
    }
}
uint64_t Clock::elapsed_ms()
{
    return elapsed_us()/1000ll;
}

uint64_t Clock::elapsed_s()
{
    return elapsed_us()/1000000ll;
}