#ifndef STATES_H
#define STATES_H

#include <stdbool.h>
#include "FiniteStateMachine.hpp"
#include "timers.hpp"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#include <string>

using namespace std;

// Helpers
bool is_grid_up(Hardware *h);
bool is_generator_up(Hardware *h);
void set_grid_contactor(Hardware *h, bool status);
void set_generator_contactor(Hardware *h, bool status);


// States

void enterUnknownStart(FSM* sm);
void updateUnknownStart(FSM* sm);
void exitUnknownStart(FSM* sm);

void enterNormal(FSM* sm);
void updateNormal(FSM* sm);
void exitNormal(FSM* sm);

void enterStability(FSM* sm);
void updateStability(FSM* sm);
void exitStability(FSM* sm);

void enterWaitGen(FSM* sm);
void updateWaitGen(FSM* sm);
void exitWaitGen(FSM* sm);

void enterWarmUp(FSM* sm);
void updateWarmUp(FSM* sm);
void exitWarmUp(FSM* sm);

void enterSwitchDelayToGen(FSM* sm);
void updateSwitchDelayToGen(FSM* sm);
void exitSwitchDelayToGen(FSM* sm);

void enterSwitchToGen(FSM* sm);
void updateSwitchToGen(FSM* sm);
void exitSwitchToGen(FSM* sm);

void enterDetachGen(FSM* sm);
void updateDetachGen(FSM* sm);
void exitDetachGen(FSM* sm);

void enterNormal2(FSM* sm);
void updateNormal2(FSM* sm);
void exitNormal2(FSM* sm);

void enterSwitchDelayToGrid(FSM* sm);
void updateSwitchDelayToGrid(FSM* sm);
void exitSwitchDelayToGrid(FSM* sm);


#endif  // STATES_H