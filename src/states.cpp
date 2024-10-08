#include "states.h"
#include <stdio.h>
#include "hardware.h"
#include "settings.h"
#include "FiniteStateMachine.hpp"


State UnknownStart = State(UNKNOWNSTART, &enterUnknownStart, &updateUnknownStart, &exitUnknownStart);
State Normal = State(NORMAL, &enterNormal, &updateNormal, &exitNormal);
State Stability = State(STABILITY, &enterStability, &updateStability, &exitStability);
State WaitGen = State(WAITGEN, &enterWaitGen, &updateWaitGen, &exitWaitGen);
State WarmUp = State(WARMUP, &enterWarmUp, &updateWarmUp, &exitWarmUp);
State SwitchDelayToGen = State(SWITCHDELAYTOGEN, &enterSwitchDelayToGen, &updateSwitchDelayToGen, &exitSwitchDelayToGen);
State SwitchToGen = State(SWITCHTOGEN, &enterSwitchToGen, &updateSwitchToGen, &exitSwitchToGen);
State DetachGen = State(DETACHGEN, &enterDetachGen, &updateDetachGen, &exitDetachGen);
State Normal2 = State(NORMAL2, &enterNormal2, &updateNormal2, &exitNormal2);
State SwitchDelayToGrid = State(SWITCHDELAYTOGRID, &enterSwitchDelayToGrid, &updateSwitchDelayToGrid, &exitSwitchDelayToGrid);


bool is_grid_up(Hardware *h) {
    int grid_voltage = h->getVoltageAC(PIN_VOLTAGE_GRID);
    if (grid_voltage < SETTING_MIN_VOLTAGE_THRESHOLD)
    {
        return false;
    }
    
    return true;
}

bool is_generator_up(Hardware *h) {
    int generator_voltage = h->getVoltageAC(PIN_VOLTAGE_GENERATOR);
    if (generator_voltage < SETTING_MIN_VOLTAGE_THRESHOLD)
    {
        return false;
    }
    
    return true;
}

void set_grid_contactor(Hardware *h, bool status) {
    // It is so critical action to ensure that the other contactor is off when
    // switching grid on, that other contactor is switched off before grid is kicked in

    // Grid is being activated, switch generator off before that
    if (status == true) {
        h->setDigital(PIN_CONTACTOR_GENERATOR, false);
        h->wait(100);
    }

    h->setDigital(PIN_CONTACTOR_GRID, status);
}

void set_generator_contactor(Hardware *h, bool status) {
    // Generator is being activated, switch grid off before that
    // and do a little guard wait
    if (status == true) {
        h->setDigital(PIN_CONTACTOR_GRID, false);
        h->wait(100);
    }

    h->setDigital(PIN_CONTACTOR_GENERATOR, status);
}


/**
 * UNKNOWNSTART -STATE
 * When defvice starts. We do not know what the state of the ATS
 * is when the Arduino starts. Find it and push state machine to
 * follow real switching board.  
*/
// This 'UNKNOWN_START'-state is used as a setup. 
// This is why timers are reseted here
void enterUnknownStart(FSM *sm) {
    sm->deps->timer->reset_timer(ALL);
    sm->transitionTo(Normal);
}

void updateUnknownStart(FSM *sm) {}

void exitUnknownStart(FSM *sm) {}


/**
 * NORMAL -STATE
 * 
*/
void enterNormal(FSM *sm) {
    // Init contactor states 
    set_generator_contactor(sm->deps->hardware, false);
    set_grid_contactor(sm->deps->hardware, true);
    // TODO LOW gen_stop
    sm->deps->timer->reset_timer(WARM_UP_TIME);
}

void updateNormal(FSM *sm) {

    // Did grid went down?
    if (is_grid_up(sm->deps->hardware) == false) {
        sm->transitionTo(Stability);
        return;
    }

    // Did generator start even if grid is up?
    if (is_generator_up(sm->deps->hardware) == true) {
        sm->transitionTo(Normal2);
    }

}

void exitNormal(FSM *sm) {}


/**
 * STABILITY -STATE
 * 
*/
void enterStability(FSM *sm) {
    // Reset stability time
    sm->deps->timer->reset_timer(STABILITY_TIME);
    // Start counting stability time from the start
    sm->deps->timer->enable_timer(STABILITY_TIME, true);
}

void updateStability(FSM *sm) {

    // Stability time counted all down to zero and grid still is not up
    if (
        sm->deps->timer->get_remaining_time(STABILITY_TIME) == 0 &&
        is_grid_up(sm->deps->hardware) == false
    ) {
        sm->transitionTo(WaitGen);
    }

    
    if (sm->getPreviousState()->getState() == NORMAL) {
        // We are coming from Normal state, so we need to check if grid is up
        // and generator is down. If so, we can go back to Normal state
        if (
            sm->deps->timer->get_remaining_time(STABILITY_TIME) == 0 &&
            is_grid_up(sm->deps->hardware) == true &&
            is_generator_up(sm->deps->hardware) == false
        ) {
            sm->transitionTo(Normal);
        }

        // Both grid and generator are up, so we can go to Normal2 state
        // to show that generator is up
        if (
            sm->deps->timer->get_remaining_time(STABILITY_TIME) == 0 &&
            is_grid_up(sm->deps->hardware) == true &&
            is_generator_up(sm->deps->hardware) == true
        ) {
            sm->transitionTo(Normal2);
        }
    }

    // If we are coming from SwitchDelayToGen state, we need to
    // go transfer delay protection before we can go to Normal state!
    if (sm->getPreviousState()->getState() == SWITCHTOGEN) {
        if (
            sm->deps->timer->get_remaining_time(STABILITY_TIME) == 0 &&
            is_grid_up(sm->deps->hardware) == true
        ) {
            sm->transitionTo(SwitchDelayToGrid);
        }
    }

    

}

void exitStability(FSM *sm) {
    sm->deps->timer->enable_timer(STABILITY_TIME, false);
}


/**
 * WAITGEN -STATE
 * 
*/
void enterWaitGen(FSM *sm) {
    sm->deps->timer->enable_timer(WARM_UP_TIME, true);
}

void updateWaitGen(FSM *sm) {

    // Do not wait that the generator starts since
    // grid went back online
    if (is_grid_up(sm->deps->hardware) == true) {
        sm->transitionTo(Stability);
    }

    if (is_generator_up(sm->deps->hardware) == true) {
        sm->transitionTo(WarmUp);
    }

}

void exitWaitGen(FSM *sm) {}


/**
 * WARMUP -STATE
 * 
*/
void enterWarmUp(FSM *sm) {
    sm->deps->timer->reset_timer(WARM_UP_TIME);
    sm->deps->timer->enable_timer(WARM_UP_TIME, true);
}

void updateWarmUp(FSM *sm) {
    if (sm->deps->timer->get_remaining_time(WARM_UP_TIME) == 0) {
        sm->transitionTo(SwitchDelayToGen);
    }
}

void exitWarmUp(FSM *sm) {
    sm->deps->timer->enable_timer(WARM_UP_TIME, false);
}


/**
 * SWITCHDELAYTOGEN -STATE
 * 
*/
void enterSwitchDelayToGen(FSM *sm) {
    // TODO CRIT Old comment: 
    // Disconnect grid contactor if generator has been
    // halted some time ago. If (generator) warm up timer
    // has not been resetted, the generator is still being used.
    // Do not remove the following line since transfer delay from 
    // grid to gen is not come true unless this line.
    set_grid_contactor(sm->deps->hardware, false);
    
    // Set timers
    sm->deps->timer->reset_timer(SWITCHING_DELAY);
    sm->deps->timer->enable_timer(SWITCHING_DELAY, true);

}

void updateSwitchDelayToGen(FSM *sm) {

    if (sm->deps->timer->get_remaining_time(SWITCHING_DELAY) == 0) {
        sm->transitionTo(SwitchToGen);
    }

}

void exitSwitchDelayToGen(FSM *sm) {
    sm->deps->timer->enable_timer(SWITCHING_DELAY, false);
}


/**
 * SWITCHTOGEN -STATE
 * 
*/
void enterSwitchToGen(FSM *sm) {
    // Connect generator to load
    set_generator_contactor(sm->deps->hardware, true);
}

void updateSwitchToGen(FSM *sm) {
    if (is_grid_up(sm->deps->hardware) == true) {
        sm->transitionTo(Stability);
    }
}

void exitSwitchToGen(FSM *sm) {}


/**
 * DETACHGEN -STATE
 * 
*/
void enterDetachGen(FSM *sm) {
    // Release generator contactor since generator is not available anymore
    set_generator_contactor(sm->deps->hardware, false);
    // Go and wait generator to start again
    sm->transitionTo(WaitGen);
}

void updateDetachGen(FSM *sm) {}

void exitDetachGen(FSM *sm) {}


/**
 * NORMAL2 -STATE
 * 
*/
void enterNormal2(FSM *sm) {}

void updateNormal2(FSM *sm) {

    if (is_generator_up(sm->deps->hardware) == false) {
        sm->transitionTo(Normal);
    }

    if (is_grid_up(sm->deps->hardware) == false) {
        sm->transitionTo(WaitGen);
    }

}

void exitNormal2(FSM *sm) {}


/**
 * SWITCHDELAYTOGRID -STATE
 * 
*/
void enterSwitchDelayToGrid(FSM *sm) {

    // Disconnect generator contactor
    set_generator_contactor(sm->deps->hardware, false);

    sm->deps->timer->reset_timer(SWITCHING_DELAY);
    sm->deps->timer->enable_timer(SWITCHING_DELAY, true);
}

void updateSwitchDelayToGrid(FSM *sm) {

    // Grid contactor is not activated here,
    // it is done by Normal-state

    if (
        sm->deps->timer->get_remaining_time(SWITCHING_DELAY) == 0 &&
        is_generator_up(sm->deps->hardware) == false
    ) {
        sm->transitionTo(Normal);
    }

    if (
        sm->deps->timer->get_remaining_time(SWITCHING_DELAY) == 0 &&
        is_generator_up(sm->deps->hardware) == true
    ) {
        sm->transitionTo(Normal2);
    }

}

void exitSwitchDelayToGrid(FSM *sm) {
    sm->deps->timer->enable_timer(SWITCHING_DELAY, false);
}