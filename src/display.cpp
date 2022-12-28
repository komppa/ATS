#include "display.h"
#include "FiniteStateMachine.hpp"
#include "settings.h"


#ifndef UNIT_TEST


State DisplayUnknownStart = State("DisplayUnknownStart", &updateDisplayUnknownStart);
State DisplayStart = State("DisplayStart", &enterDisplayStart, &updateDisplayStart, &exitDisplayStart);
State SettingsStart = State("SettingsStart", &enterSettingsStart, &updateSettingsStart, &exitSettingsStart);
State SettingsStabilityTime = State("SettingsStabilityTime", &enterSettingsStabilityTime, &updateSettingsStabilityTime, &exitSettingsStabilityTime);


/**
 * DISPLAYUNKNOWNSTART -STATE
 * 
*/
void updateDisplayUnknownStart(FSM *dsm) {
    
    // Show startup screen
    dsm->deps->writer->write("AUTOMATIC", "TRANSFER SWITCH");
    delay(2000);
    dsm->deps->writer->write(
        "VERSION " + (String)SW_VERSION,
        "K&I ENT."
    );
    delay(1000);
    dsm->deps->writer->clear();

    // This is update - immediately transfer me away from here
    dsm->immediateTransitionTo(DisplayStart);

}


/**
 * DISPLAYSTART -STATE
 * 
*/
void enterDisplayStart(FSM* dsm) {



}

void updateDisplayStart(FSM* dsm) {


    char key = DISPLAY_GET_KEY;
    
    if (key == '#') {
        dsm->transitionTo(SettingsStart);
    }

    dsm->deps->writer->write(
        dsm->deps->sm->getCurrentState().getStateName(), ""
    );


}

void exitDisplayStart(FSM* dsm) {}


/**
 * SETTINGSSTART -STATE
 * 
*/
void enterSettingsStart(FSM* dsm) {

    dsm->deps->writer->write(
        ">>> SETTINGS <<<",
        "Press #- to navigate"
    );

}

void updateSettingsStart(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(DisplayStart);
    }

}

void exitSettingsStart(FSM* dsm) {}


/**
 * SETTINGSSTABILITYTIME -STATE
 * 
*/
void enterSettingsStabilityTime(FSM* dsm) {

    dsm->deps->writer->write(
        ">>> SETTINGS <<<",
        "Press #- to navigate"
    );

}

void updateSettingsStabilityTime(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(DisplayStart);
    }

}

void exitSettingsStabilityTime(FSM* dsm) {}


#endif // UNIT_TEST