#include "display.h"
#include "FiniteStateMachine.hpp"
#include "settings.h"


#ifndef UNIT_TEST


State DisplayUnknownStart = State("DisplayUnknownStart", DISPLAYUNKNOWNSTART, &updateDisplayUnknownStart);
State DisplayStart = State("DisplayStart", DISPLAYSTART, &enterDisplayStart, &updateDisplayStart, &exitDisplayStart);
State SettingsStart = State("SettingsStart", SETTINGSSTART, &enterSettingsStart, &updateSettingsStart, &exitSettingsStart);
State SettingsStabilityTime = State("SettingsStabilityTime", SETTINGSSTABILITYTIME, &enterSettingsStabilityTime, &updateSettingsStabilityTime, &exitSettingsStabilityTime);


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
void enterDisplayStart(FSM* dsm) {}

void updateDisplayStart(FSM* dsm) {

    char key = DISPLAY_GET_KEY;
    
    if (key == '#') {
        dsm->transitionTo(SettingsStart);
    }

    switch (dsm->deps->sm->getCurrentState().getState()) {
        case UNKNOWNSTART:
            dsm->deps->writer->write("ATS BOOTING...");
            break;
        case NORMAL:
            dsm->deps->writer->write(
                (String)"GRID UP    " + dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GRID),
                (String)"GENSET OFF " + dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GENERATOR)
            );
            break;
        case NORMAL2:
            dsm->deps->writer->write(
                (String)"GRID UP    " + dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GRID),
                (String)"GENSET ON " + dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GENERATOR)
            );
            break;
        case STABILITY:
            dsm->deps->writer->write(
                (String)"GRID WENT DOWN",   // TODO CRIT if coming from waitgen, wrong text
                (String)"STABILITY " + dsm->deps->timer->get_remaining_time(STABILITY_TIME)
            );
            break;
        case WAITGEN:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        case WARMUP:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        case SWITCHDELAYTOGEN:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        case SWITCHTOGEN:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        case SWITCHDELAYTOGRID:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        case DETACHGEN:
            dsm->deps->writer->write(
                dsm->deps->sm->getCurrentState().getStateName()
            );
            break;
        default:
            break;
    }

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