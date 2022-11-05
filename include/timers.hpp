#ifndef TIMERS_HPP
#define TIMERS_HPP


#include <stdbool.h>
#include "hardware.h"

// All these timers are in seconds
struct Timers {
    bool stability_time_active;
    unsigned long remaining_stability_time;
    unsigned long initial_stability_time;

    bool switching_delay_active;
    int remaining_switching_delay;
    int initial_switching_delay;

    bool warm_up_time_active;
    int remaining_warm_up_time;
    int initial_warm_up_time;

    unsigned long last_timer_run;   // When timer was last run (used against millis)
};

enum TimerSelection {
    ALL,
    STABILITY_TIME,
    SWITCHING_DELAY,
    WARM_UP_TIME
};


class Timer {
    private:
        Timers timers;
        Hardware *hardware;
    public:
        Timer();
        Timer(Hardware*);
        virtual void set_timer(TimerSelection, int);
        virtual int reset_timer(TimerSelection);
        virtual int update();
        virtual int get_remaining_time(TimerSelection);
        virtual int get_initial_time(TimerSelection);
        virtual void enable_timer(TimerSelection, bool);
};

#ifdef UNIT_TEST
class MockTimer: public Timer {
    public:
    MOCK_METHOD(int, set_timer, ());
    MOCK_METHOD(int, reset_timer, ());
    MOCK_METHOD(int, update, ());
    MOCK_METHOD(int, get_remaining_time, ());
    MOCK_METHOD(int, get_initial_time, ());
    MOCK_METHOD(void, enable_timer, ());
};
#endif // UNIT_TEST

#endif