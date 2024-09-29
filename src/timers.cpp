#include "timers.hpp"


Timer::Timer() {
    this->timers = (Timers){
        .stability_time_active = false,
        .remaining_stability_time = 30,
        .initial_stability_time = 30,

        .switching_delay_active = false,
        .remaining_switching_delay = 5,
        .initial_switching_delay = 5,

        .warm_up_time_active = false,
        .remaining_warm_up_time = 30,
        .initial_warm_up_time = 30,

        .last_timer_run = 0
    };
}

Timer::Timer(Hardware *hardware) {
    this->timers = (Timers){
        .stability_time_active = false,
        .remaining_stability_time = 30,
        .initial_stability_time = 30,

        .switching_delay_active = false,
        .remaining_switching_delay = 5,
        .initial_switching_delay = 5,

        .warm_up_time_active = false,
        .remaining_warm_up_time = 30,
        .initial_warm_up_time = 30,

        .last_timer_run = 0,
    };
    this->hardware = hardware;
};

int Timer::get_remaining_time(TimerSelection timer) {
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
 * Returns the start time for each timer, without any modifications
 * (like ticking down the time).
 */
int Timer::get_initial_time(TimerSelection timer) {
    switch (timer) {
        case STABILITY_TIME:
            return this->timers.initial_stability_time;
        case SWITCHING_DELAY:
            return this->timers.initial_switching_delay;
        case WARM_UP_TIME:
            return this->timers.initial_warm_up_time;
        default:
            return -1;
    }
}

/**
 * Enable or disable timer counter when calling update method.
*/
void Timer::enable_timer(TimerSelection timer, bool active) {
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

/**
 * Set new initial/default time for the spcific timer.
*/
void Timer::set_timer(TimerSelection timer, int initial_time) {
    switch (timer) {
        case STABILITY_TIME:
            this->timers.initial_stability_time = initial_time;
            // TODO ArduinoFunc class with mocks
            break;
        case SWITCHING_DELAY:
            this->timers.initial_switching_delay = initial_time;
            break;
        case WARM_UP_TIME:
            this->timers.initial_warm_up_time = initial_time;
            break;
        case ALL:
            this->timers.initial_stability_time = initial_time;
            this->timers.initial_switching_delay = initial_time;
            this->timers.initial_warm_up_time = initial_time;
        default:
            break;
    }
}


/**
 * Reset timer(s) remaining value to be initial value and
 * setting timer activity (enabled state) to be disabled.
*/
int Timer::reset_timer(TimerSelection timer_selection = ALL) {
    switch (timer_selection) {
        case ALL:
            this->timers.stability_time_active = false;
            this->timers.remaining_stability_time = this->timers.initial_stability_time;

            this->timers.switching_delay_active = false;
            this->timers.remaining_switching_delay = this->timers.initial_switching_delay;

            this->timers.warm_up_time_active = false;
            this->timers.remaining_warm_up_time = this->timers.initial_warm_up_time;
            break;
        case STABILITY_TIME:
            this->timers.stability_time_active = false;
            this->timers.remaining_stability_time = this->timers.initial_stability_time;
            break;
        case SWITCHING_DELAY:
            this->timers.switching_delay_active = false;
            this->timers.remaining_switching_delay = this->timers.initial_switching_delay;
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


/**
 * Check whether enough time has passed that remaining times of the 
 * timers could be ticked down. 
*/
int Timer::update(void) {

    if ((unsigned long)(this->hardware->time() - this->timers.last_timer_run) >= 1000) {
        this->timers.last_timer_run = this->hardware->time();

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

