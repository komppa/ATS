#ifndef STATES_H
#define STATES_H

#include <stdbool.h>
#include "FiniteStateMachine.hpp"
#include "timers.hpp"


void init_state_machine();


// Helpers
bool isGridUp();
bool isGeneratorUp();
void setGridContactor(bool status);
void setGeneratorContactor(bool status);


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