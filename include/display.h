#ifndef DISPLAY_H
#define DISPLAY_H

#include "FiniteStateMachine.hpp"

#ifndef UNIT_TEST
#include <Arduino.h>
#include "settings.h"


#define DISPLAY_GET_KEY dsm->deps->keypad->getKey()

void drawTemplate(Writer *writer, Hardware *hardware, State *state);


// States

void updateDisplayUnknownStart(FSM *dsm);

void enterDisplayStart(FSM* dsm);
void updateDisplayStart(FSM* dsm);
void exitDisplayStart(FSM* dsm);

void enterSettingsStart(FSM* dsm);
void updateSettingsStart(FSM* dsm);
void exitSettingsStart(FSM* dsm);

void enterManualDrive(FSM* dsm);
void updateManualDrive(FSM* dsm);
void exitManualDrive(FSM* dsm);

void enterSettingsStabilityTime(FSM* dsm);
void updateSettingsStabilityTime(FSM* dsm);
void exitSettingsStabilityTime(FSM* dsm);

void enterSwitchingDelay(FSM* dsm);
void updateSwitchingDelay(FSM* dsm);
void exitSwitchingDelay(FSM* dsm);

void enterWarmUpTime(FSM* dsm);
void updateWarmUpTime(FSM* dsm);
void exitWarmUpTime(FSM* dsm);

void enterCalibrateZeroVoltage(FSM* dsm);
void updateCalibrateZeroVoltage(FSM* dsm);
void exitCalibrateZeroVoltage(FSM* dsm);

void enterCalibrateKnownVoltage(FSM* dsm);
void updateCalibrateKnownVoltage(FSM* dsm);
void exitCalibrateKnownVoltage(FSM* dsm);

void enterSettingsInput(FSM* dsm);
void updateSettingsInput(FSM* dsm);
void exitSettingsInput(FSM* dsm);

#endif  // UNIT_TEST

#endif  // DISPLAY_H