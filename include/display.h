#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef UNIT_TEST
#include "FiniteStateMachine.hpp"
#endif

#ifndef UNIT_TEST
#include <Arduino.h>



#define DISPLAY_GET_KEY dsm->deps->keypad->getKey()

// States

void updateDisplayUnknownStart(FSM *dsm);

void enterDisplayStart(FSM* dsm);
void updateDisplayStart(FSM* dsm);
void exitDisplayStart(FSM* dsm);

void enterSettingsStart(FSM* dsm);
void updateSettingsStart(FSM* dsm);
void exitSettingsStart(FSM* dsm);

void enterSettingsStabilityTime(FSM* dsm);
void updateSettingsStabilityTime(FSM* dsm);
void exitSettingsStabilityTime(FSM* dsm);


#endif  // UNIT_TEST

#endif  // DISPLAY_H