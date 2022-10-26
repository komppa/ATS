#ifndef TIMERS_HPP
#define TIMERS_HPP


#include <stdbool.h>

// All these timers are in seconds
struct Timers {
    bool stability_time_active;
    unsigned long remaining_stability_time;
    unsigned long intial_stability_time;

    bool switching_delay_active;
    int remaining_switching_delay;
    int intial_switching_delay;

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
    public:
        Timer();
        int reset_timer(TimerSelection);
        int update();
        int get_timer_time(TimerSelection);
        void set_timer(TimerSelection, bool);
};


#endif