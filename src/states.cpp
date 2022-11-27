#include "states.h"
#include <stdio.h>
#include "hardware.h"
#include "settings.h"

Timer timer;


State UnknownStart = State("UnknownStart", enterUnknownStart, updateUnknownStart, exitUnknownStart);
State Normal = State("Normal", enterNormal, updateNormal, exitNormal);
State Stability = State("Stability", enterStability, updateStability, exitStability);
State WaitGen = State("WaitGen", enterWaitGen, updateWaitGen, exitWaitGen);
State WarmUp = State("WarmUp", enterWarmUp, updateWarmUp, exitWarmUp);
State SwitchDelayToGen = State("SwitchDelayToGen", enterSwitchDelayToGen, updateSwitchDelayToGen, exitSwitchDelayToGen);
State SwitchToGen = State("SwitchToGen", enterSwitchToGen, updateSwitchToGen, exitSwitchToGen);
State DetachGen = State("DetachGen", enterDetachGen, updateDetachGen, exitDetachGen);
State Normal2 = State("Normal2", enterNormal2, updateNormal2, exitNormal2);
State SwitchDelayToGrid = State("SwitchDelayToGrid", enterSwitchDelayToGrid, updateSwitchDelayToGrid, exitSwitchDelayToGrid);


FSM sm = FSM(UnknownStart);
Hardware hardware;


bool is_grid_up() {
    int grid_voltage = hardware.getVoltageAC(PIN_VOLTAGE_GRID);
    if (grid_voltage < SETTING_MIN_VOLTAGE_THRESHOLD)
    {
        return false;
    }
    
    return true;
}

bool is_generator_up() {
    int grid_voltage = hardware.getVoltageAC(PIN_VOLTAGE_GRID);
    if (grid_voltage < SETTING_MIN_VOLTAGE_THRESHOLD)
    {
        return false;
    }
    
    return true;
}

void set_grid_contactor(bool status) {
    // It is so critical action to ensure that the other contactor is off when
    // switching grid on, that other contactor is switched off before grid is kicked in

    // Grid is being activated, switch generator off before that
    if (status == true) {
        hardware.digitalWrite(PIN_CONTACTOR_GENERATOR, false);
        hardware.delay(100);
    }

    hardware.digitalWrite(PIN_CONTACTOR_GRID, status);
}

void set_generator_contactor(bool status) {
    // Generator is being activated, switch grid off before that
    // and do a little guard wait
    if (status == true) {
        hardware.digitalWrite(PIN_CONTACTOR_GRID, false);
        hardware.delay(100);
    }

    hardware.digitalWrite(PIN_CONTACTOR_GENERATOR, status);
}


/**
 * UNKNOWNSTART -STATE
 * When defvice starts. We do not know what the state of the ATS
 * is when the Arduino starts. Find it and push state machine to
 * follow real switching board.  
*/
// This 'UNKNOWN_START'-state is used as a setup. 
// This is why timers are reseted here
void enterUnknownStart() {
    // TODO CRIT TODO HIGH check really and not just forward a call
    timer.reset_timer(ALL);
    sm.transitionTo(Normal);
}

void updateUnknownStart() {
    printf("@updateUnknownStart\n");

}

void exitUnknownStart() {
    printf("@exitUnknownStart\n");

}


/**
 * NORMAL -STATE
 * 
*/
void enterNormal() {

    printf("@enterNormal\n");

    // Init contactor states 
    set_generator_contactor(false);
    set_grid_contactor(true);
    // TODO LOW gen_stop

}

void updateNormal() {


    // Did grid went down?
    if (is_grid_up() == false) {
        sm.transitionTo(Stability);
    }

    // Did generator start even if grid is up?
    if (is_generator_up() == true) {
        sm.transitionTo(Normal2);
    }

}

void exitNormal() {

}


/**
 * STABILITY -STATE
 * 
*/
void enterStability() {
    // Reset stability time
    timer.reset_timer(STABILITY_TIME);
    // Start counting stability time from the start
    timer.set_timer(STABILITY_TIME, true);
}

void updateStability() {

    // Stability time counted all down to zero and grid still is not up
    if (timer.get_remaining_time(STABILITY_TIME) == 0 && is_grid_up() == false) {
        sm.transitionTo(WaitGen);
    }

    if (
        timer.get_remaining_time(STABILITY_TIME) == 0 &&
        is_grid_up() == true &&
        is_generator_up() == false
    ) {
        sm.transitionTo(Normal);
    }

    if (
        is_grid_up() == true &&
        is_generator_up() == true &&
        timer.get_remaining_time(STABILITY_TIME) == 0
    ) {
        sm.transitionTo(SwitchDelayToGrid);
    }

}

void exitStability() {
    timer.set_timer(STABILITY_TIME, false);
}


/**
 * WAITGEN -STATE
 * 
*/
void enterWaitGen() {

}

void updateWaitGen() {

    // Do not wait that the generator starts since
    // grid went back online
    if (is_grid_up() == true) {
        sm.transitionTo(Stability);
    }

    if (is_generator_up() == true) {
        sm.transitionTo(WarmUp);
    }

}

void exitWaitGen() {

}


/**
 * WARMUP -STATE
 * 
*/
void enterWarmUp() {
    timer.reset_timer(WARM_UP_TIME);
    timer.set_timer(WARM_UP_TIME, true);
}

void updateWarmUp() {
    if (timer.get_remaining_time(WARM_UP_TIME) == 0) {
        sm.transitionTo(SwitchDelayToGen);
    }
}

void exitWarmUp() {
    timer.set_timer(WARM_UP_TIME, false);
}


/**
 * SWITCHDELAYTOGEN -STATE
 * 
*/
void enterSwitchDelayToGen() {
    // Disconnect grid contactor
    set_grid_contactor(false);
    // Set timers
    timer.reset_timer(SWITCHING_DELAY);
    timer.set_timer(SWITCHING_DELAY, true);

}

void updateSwitchDelayToGen() {

    if (timer.get_remaining_time(SWITCHING_DELAY) == 0) {
        sm.transitionTo(SwitchToGen);
    }

}

void exitSwitchDelayToGen() {
    timer.set_timer(SWITCHING_DELAY, false);
}


/**
 * SWITCHTOGEN -STATE
 * 
*/
void enterSwitchToGen() {

    // Connect generator to load
    set_generator_contactor(true);

    // This state is not fully implemented as specification suggests.
    // Specification is too inaccurate. >We assume that the generator keeps running
    sm.transitionTo(Stability);

}

void updateSwitchToGen() {
    // Not used, check is made instanly on @enter
}

void exitSwitchToGen() {

}


/**
 * DETACHGEN -STATE
 * 
*/
void enterDetachGen() {
    // Not used.
    // But if someone uses, kick instantly to next (only one possible next) state
    sm.transitionTo(WaitGen);
}

void updateDetachGen() {

}

void exitDetachGen() {

}


/**
 * NORMAL2 -STATE
 * 
*/
void enterNormal2() {

}

void updateNormal2() {

    if (is_generator_up() == false) {
        sm.transitionTo(Normal);
    }

    if (is_grid_up() == false) {
        sm.transitionTo(WaitGen);
    }

}

void exitNormal2() {

}


/**
 * SWITCHDELAYTOGRID -STATE
 * 
*/
void enterSwitchDelayToGrid() {
    timer.reset_timer(SWITCHING_DELAY);
    timer.set_timer(SWITCHING_DELAY, true);
}

void updateSwitchDelayToGrid() {

    if (
        is_grid_up() == true &&
        is_generator_up() == true &&
        timer.get_remaining_time(STABILITY_TIME) == 0
    ) {
        sm.transitionTo(Normal);
    }

}

void exitSwitchDelayToGrid() {
    timer.set_timer(SWITCHING_DELAY, false);
}