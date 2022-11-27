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

void init_state_machine();


// Helpers
bool is_grid_up();
bool is_generator_up();
void set_grid_contactor(bool status);
void set_generator_contactor(bool status);


// States

void enterUnknownStart();
void updateUnknownStart();
void exitUnknownStart();

void enterNormal();
void updateNormal();
void exitNormal();

void enterStability();
void updateStability();
void exitStability();

void enterWaitGen();
void updateWaitGen();
void exitWaitGen();

void enterWarmUp();
void updateWarmUp();
void exitWarmUp();

void enterSwitchDelayToGen();
void updateSwitchDelayToGen();
void exitSwitchDelayToGen();

void enterSwitchToGen();
void updateSwitchToGen();
void exitSwitchToGen();

void enterDetachGen();
void updateDetachGen();
void exitDetachGen();

void enterNormal2();
void updateNormal2();
void exitNormal2();

void enterSwitchDelayToGrid();
void updateSwitchDelayToGrid();
void exitSwitchDelayToGrid();


#endif