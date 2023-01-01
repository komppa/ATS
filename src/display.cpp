#include "display.h"
#include "FiniteStateMachine.hpp"
#include "settings.h"


#ifndef UNIT_TEST


State DisplayUnknownStart = State("DisplayUnknownStart", DISPLAYUNKNOWNSTART, &updateDisplayUnknownStart);
State DisplayStart = State("DisplayStart", DISPLAYSTART, &enterDisplayStart, &updateDisplayStart, &exitDisplayStart);
State SettingsStart = State("SettingsStart", SETTINGSSTART, &enterSettingsStart, &updateSettingsStart, &exitSettingsStart);
State SettingsStabilityTime = State("SettingsStabilityTime", SETTINGSSTABILITYTIME, &enterSettingsStabilityTime, &updateSettingsStabilityTime, &exitSettingsStabilityTime);


void drawTemplate(Writer *writer, Hardware *hardware, State *state) {
    switch (state->getState()) {
        case UNKNOWNSTART:
            writer->write("ATS BOOTING {5}", "{5}");
            break;
        case NORMAL:
            writer->write(
                "# GRID UP   {3}V",
                "% GEN DOWN  {3}V"
            );
            break;
        case NORMAL2:
            writer->write(
                "# GRID UP   {3}V",
                "% GEN UP    {3}V"
            );
            break;
        case STABILITY:
            writer->setMode(MIDDLE);
            if (hardware->getVoltageAC(PIN_VOLTAGE_GRID) < SETTING_MIN_VOLTAGE_THRESHOLD) {
                writer->write(
                    "GRID WENT DOWN! CHECKING STABILITY IN:",
                    "{3}S"
                );
                return;
            }
            writer->write(
                "RESTORING GRID! CHECKING STABILITY IN:",
                "{3}S"
            );
            
            break;
        case WAITGEN:
            writer->setMode(MIDDLE);
            writer->write(
                "WAITING GEN TO BE STARTED",
                "OR  GRID TO BE RESTORED"
            );
            break;
        case WARMUP:
            writer->write(
                "WAITING GEN TO WARM UP",
                "{3}S"
            );
            break;
        case SWITCHDELAYTOGEN:
            writer->write(
                "TRANSFER DELAY",
                "{3}S"
            );
            break;
        case SWITCHTOGEN:
            writer->write(
                "* NOW USING GENERATOR *",
                "SWITCHING BACK TO GRID IF RESTORES"
            );
            break;
        case SWITCHDELAYTOGRID:
            writer->write(
                "TRANSFER DELAY",
                "{3}S"
            );
            break;
        case DETACHGEN:
            writer->write(
                " {3}",
                " {3}"
            );
            break;
        default:
            writer->write((String)"STATE \"" + state->getStateName() + "\" {5}", "IS UNKNOWN, BUG? {5}");
            break;
    }
}

/**
 * DISPLAYUNKNOWNSTART -STATE
 * 
*/
void updateDisplayUnknownStart(FSM *dsm) {
    
    // Show startup screen if is not a simulator
    if (dsm->deps->hardware->isSimulator() == false) {
        dsm->deps->writer->write("AUTOMATIC", "TRANSFER SWITCH");
        delay(2000);
        dsm->deps->writer->write(
            "VERSION " + (String)SW_VERSION,
            "K&I ENT."
        );
        delay(1000);
        dsm->deps->writer->clear();
    }
    

    // This is update - immediately transfer me away from here
    dsm->immediateTransitionTo(DisplayStart);

}


/**
 * DISPLAYSTART -STATE
 * 
*/
void enterDisplayStart(FSM* dsm) {

    dsm->deps->writer->setMode(RAW);

    // switch (dsm->deps->sm->getCurrentState().getState())
    // {
    // case NORMAL:
    //     dsm->deps->writer->write(
    //         "GRID UP {5} V",
    //         "GENSET DOWN {3} V"
    //     );
    //     break;
    // case NORMAL2:
    //     dsm->deps->writer->write(
    //         "GRID UP {3} V",
    //         "GENSET UP {3} V"
    //     );
    //     break;   
    // case STABILITY:
    //     dsm->deps->writer->write(
    //         "GRID WENT DOWN - STARTING GENSET IF NO POWER RESTORE",
    //         "STABILITY {1}"
    //     );
    
    // default:
    //     break;
    // }
    
}

void updateDisplayStart(FSM* dsm) {

    if (dsm->deps->sm->changePending() == true) {
        drawTemplate(
            dsm->deps->writer,
            dsm->deps->hardware,
            &dsm->deps->sm->getCurrentState()
        );
        dsm->deps->sm->clearPendingFlag();
    }

    char key = DISPLAY_GET_KEY;
    
    if (key == '#') {
        dsm->transitionTo(SettingsStart);
    }

    switch (dsm->deps->sm->getCurrentState().getState()) {
        case UNKNOWNSTART:
            dsm->deps->writer->write("ATS BOOTING...");
            break;
        case NORMAL:
            dsm->deps->writer->variable(
                FIRST,
                (String)dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GRID)
            );
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GENERATOR)
            );
            break;
        case NORMAL2:
            dsm->deps->writer->variable(
                FIRST,
                (String)dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GRID)
            );
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->hardware->getVoltageAC(PIN_VOLTAGE_GENERATOR)
            );
            break;
        case STABILITY:
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->timer->get_remaining_time(STABILITY_TIME)        
            );
            break;
        case WAITGEN:
            break;
        case WARMUP:
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->timer->get_remaining_time(WARM_UP_TIME)
            );
            break;
        case SWITCHDELAYTOGEN:
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->timer->get_remaining_time(SWITCHING_DELAY)
            );
            break;
        case SWITCHTOGEN:
            break;
        case SWITCHDELAYTOGRID:
            dsm->deps->writer->variable(
                SECOND,
                (String)dsm->deps->timer->get_remaining_time(SWITCHING_DELAY)
            );
            break;
        case DETACHGEN:
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

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        ">>> SETTINGS <<<",
        "Press #- to cycle settings or *- to quit"
    );

}

void updateSettingsStart(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(DisplayStart);
    }

    if (key == '*') {
        dsm->transitionTo(DisplayStart);
    }

}

void exitSettingsStart(FSM* dsm) {}


/**
 * SETTINGSSTABILITYTIME -STATE
 * 
*/
void enterSettingsStabilityTime(FSM* dsm) {

    dsm->deps->writer->clear();
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