#include "external_time.h"  // For getTime()
#include "timers.hpp"


// Default values before they are got i.e. from EEPROM
// Timers timers = (Timers){
//     .stability_time_active = false,
//     .remaining_stability_time = 30,
//     .intial_stability_time = 30,

//     .switching_delay_active = false,
//     .remaining_switching_delay = 5,
//     .intial_switching_delay = 5,

//     .warm_up_time_active = false,
//     .remaining_warm_up_time = 30,
//     .initial_warm_up_time = 30,

//     .last_timer_run = 0,
// };


Timer::Timer() {
    this->timers = (Timers){
        .stability_time_active = false,
        .remaining_stability_time = 30,
        .intial_stability_time = 30,

        .switching_delay_active = false,
        .remaining_switching_delay = 5,
        .intial_switching_delay = 5,

        .warm_up_time_active = false,
        .remaining_warm_up_time = 30,
        .initial_warm_up_time = 30,

        .last_timer_run = 0,
    };
};

int Timer::get_timer_time(TimerSelection timer) {
    switch (timer) {
        case STABILITY_TIME:
            return this->timers.remaining_stability_time;
        case SWITCHING_DELAY:
            return this->timers.remaining_switching_delay;
        case WARM_UP_TIME:
            return this->timers.remaining_warm_up_time;
        default:
            return -1;
    }
}

/**
 * Enable or disable the timer counter by using update method
*/
void Timer::set_timer(TimerSelection timer, bool active) {
    switch (timer) {
        case STABILITY_TIME:
            this->timers.stability_time_active = active;
            break;
        case SWITCHING_DELAY:
            this->timers.switching_delay_active = active;
            break;
        case WARM_UP_TIME:
            this->timers.warm_up_time_active = active;
            break;
        default:
            break;
    }
}

int Timer::reset_timer(TimerSelection timer_selection = ALL) {
    switch (timer_selection) {
        case ALL:
            this->timers.stability_time_active = false;
            this->timers.remaining_stability_time = this->timers.intial_stability_time;

            this->timers.switching_delay_active = false;
            this->timers.remaining_switching_delay = this->timers.intial_switching_delay;

            this->timers.warm_up_time_active = false;
            this->timers.remaining_warm_up_time = this->timers.initial_warm_up_time;
            break;
        case STABILITY_TIME:
            this->timers.stability_time_active = false;
            this->timers.remaining_stability_time = this->timers.intial_stability_time;
            break;
        case SWITCHING_DELAY:
            this->timers.switching_delay_active = false;
            this->timers.remaining_switching_delay = this->timers.intial_switching_delay;
            break;
        case WARM_UP_TIME:
            this->timers.warm_up_time_active = false;
            this->timers.remaining_warm_up_time = this->timers.initial_warm_up_time;
            break;
        default:
            break;
    }

    return 0;
}

int Timer::update(void) {

    if ((unsigned long)(get_time() - this->timers.last_timer_run) > 1000) {
        this->timers.last_timer_run = get_time();

        // Reduce every counter once a second if they are active
        // and they are not already counted all way down

        if (
            this->timers.stability_time_active == true &&
            this->timers.remaining_stability_time > 0
        ) {
            this->timers.remaining_stability_time--;
        }

        if (
            this->timers.switching_delay_active == true &&
            this->timers.remaining_switching_delay > 0
        ) {
            this->timers.remaining_switching_delay--;
        }

        if (
            this->timers.warm_up_time_active == true &&
            this->timers.remaining_warm_up_time > 0
        ) {
            this->timers.remaining_warm_up_time--;
        }

        
    }

    return 0;
    
}

