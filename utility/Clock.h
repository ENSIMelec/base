#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <ctime>
#include <cstdint>
#include <sys/time.h>

class Clock
{
public:

	Clock() { restart(); }

    uint64_t getMicros(timeval t);

    void pause();
    void resume();
    void restart();
    
    uint64_t elapsed_us();
    uint64_t elapsed_ms();
    uint64_t elapsed_s();

private:
    uint64_t _start_time{};
    uint64_t _paused_time{};
    bool paused = false;
};

#endif //CLOCK_H_INCLUDED