#include "display.h"
#include "FiniteStateMachine.hpp"
#include "settings.h"


#ifndef UNIT_TEST


State DisplayUnknownStart = State("DisplayUnknownStart", DISPLAYUNKNOWNSTART, &updateDisplayUnknownStart);
State DisplayStart = State("DisplayStart", DISPLAYSTART, &enterDisplayStart, &updateDisplayStart, &exitDisplayStart);
State SettingsStart = State("SettingsStart", SETTINGSSTART, &enterSettingsStart, &updateSettingsStart, &exitSettingsStart);
State SettingsManualDrive = State("SettingsManualDrive", SETTINGSMANUALDRIVE, &enterManualDrive, &updateManualDrive, &exitManualDrive);
State SettingsStabilityTime = State("SettingsStabilityTime", SETTINGSSTABILITYTIME, &enterSettingsStabilityTime, &updateSettingsStabilityTime, &exitSettingsStabilityTime);
State SettingsSwitchingDelay = State("SettingsSwitchingDelay", SETTINGSSWITCHINGDELAY, &enterSwitchingDelay, &updateSwitchingDelay, &exitSwitchingDelay);
State SettingsWarmUpTime = State("SettingsWarmUpTime", SETTINGSWARMUPTIME, &enterWarmUpTime, &updateWarmUpTime, &exitWarmUpTime);
State SettingsInput = State("SettingsInput", SETTINGSINPUT, &enterSettingsInput, &updateSettingsInput, &exitSettingsInput);


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

bool key_is_number(char key) {
    if (
        key == '0' ||
        key == '1' ||
        key == '2' ||
        key == '3' ||
        key == '4' ||
        key == '5' ||
        key == '6' ||
        key == '7' ||
        key == '8' ||
        key == '9'
    ) {
        return true;
    }
    return false;
}

/**
 * DISPLAYUNKNOWNSTART -STATE
 * 
*/
void updateDisplayUnknownStart(FSM *dsm) {
    
    // Show startup screen if is not a simulator
    if (dsm->deps->hardware->isSimulator() == false) {
        dsm->deps->writer->setMode(MIDDLE);
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
 * SettingsStart -STATE
 * 
*/
void enterSettingsStart(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        ">>SETTINGS<< 0/4",
        "Press #- to cycle settings"
    );

}

void updateSettingsStart(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(SettingsManualDrive);
    }

    if (key == '*') {
        dsm->transitionTo(DisplayStart);
    }

}

void exitSettingsStart(FSM* dsm) {}


/**
 * MANUALDRIVE -STATE
 * 
*/
void enterManualDrive(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->setMode(MIDDLE);
    dsm->deps->writer->write(
        "MANUAL DRIVE 1/4",
        "Press to manually use: 1=GRID or 2=GENERATOR"
    );

}

void updateManualDrive(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(SettingsStabilityTime);
    }

}

void exitManualDrive(FSM* dsm) {}


/**
 * SETTINGSSTABILITYTIME -STATE
 * 
*/
void enterSettingsStabilityTime(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "STABILITY TI 2/4",
        "Change current value \"{3}\" by pressing * or press # to continue"
    );

}

void updateSettingsStabilityTime(FSM* dsm) {

    char key = DISPLAY_GET_KEY;
    String key_buffer = "";

    dsm->deps->writer->variable(
        SECOND,
        (String)dsm->deps->timer->get_remaining_time(STABILITY_TIME)
    );

    if (key == '#') {
        dsm->transitionTo(SettingsSwitchingDelay);
    }

    if (key == '*') {
        dsm->transitionTo(SettingsInput);
    }

}

void exitSettingsStabilityTime(FSM* dsm) {}

/**
 * SWITCHINGDELAY -STATE
 * 
*/
void enterSwitchingDelay(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "SWITCH DELAY 3/4",
        "Press #- to navigate"
    );

}

void updateSwitchingDelay(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        dsm->transitionTo(SettingsWarmUpTime);
    }

    if (key == '*') {
        dsm->transitionTo(SettingsInput);
    }

}

void exitSwitchingDelay(FSM* dsm) {}


/**
 * WARMUPTIME -STATE
 * 
*/
void enterWarmUpTime(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "WARM UP TIME 4/4",
        "Press #- to navigate"
    );

}

void updateWarmUpTime(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        // ATS FSM should show for DisplayState that there are state
        // state change pending and DisplayState would redraw template
        // again
        dsm->deps->sm->forceTemplateReDraw();
        
        dsm->transitionTo(DisplayUnknownStart);
    }

    if (key == '*') {
        dsm->transitionTo(SettingsInput);
    }

}

void exitWarmUpTime(FSM* dsm) {
    dsm->deps->writer->clear();
    dsm->deps->writer->setMode(RAW);
}


/**
 * SETTINGSINPUT -STATE
 * 
*/
void enterSettingsInput(FSM* dsm) {

    dsm->deps->settings->clear_num_buffer();

    switch (dsm->getPreviousState()->getState()) {
        case SETTINGSMANUALDRIVE:
            dsm->deps->writer->write(
                "Type new value for UNIMPLEMENTED",
                "yet"
            );
            break;
        case SETTINGSSTABILITYTIME:
            dsm->deps->writer->write(
                "Type new value for stability time and press # to save or press * to cancel without saving",
                "{3}"
            );
            break;
        case SETTINGSSWITCHINGDELAY:
            break;
        case SETTINGSWARMUPTIME:
            break;
        default:
            Serial.println("deautl");
            dsm->transitionTo(DisplayStart);
            break;
        
    }

}

void updateSettingsInput(FSM* dsm) {

    char key = DISPLAY_GET_KEY;
    String key_buffer = dsm->deps->settings->get_num_buffer_string();

    dsm->deps->writer->variable(
        SECOND,
        key_buffer
    );

    if (key_is_number(key) == true) {
        int number = key - '0';
        dsm->deps->settings->add_num_buffer(number);
    }

    if (key == '#') {
        dsm->deps->settings->commit_setting(
            // TODO CRIT
            // dsm->deps->hardware,
            dsm->getPreviousState()->getState(),
            key_buffer
        );
    }

    if (key == '*') {
        // Cancel pressed, go back wherever you came from
        // by using getter of previous state
        dsm->transitionTo(
            *dsm->getPreviousState()
        );
    }

}

void exitSettingsInput(FSM* dsm) {
    dsm->deps->writer->clear();
    dsm->deps->writer->setMode(RAW);
}

#endif // UNIT_TEST