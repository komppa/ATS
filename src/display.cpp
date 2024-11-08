#include "display.h"
#include "FiniteStateMachine.hpp"
#include "settings.h"


#ifndef UNIT_TEST


State DisplayUnknownStart = State(DISPLAYUNKNOWNSTART, &updateDisplayUnknownStart);
State DisplayStart = State(DISPLAYSTART, &enterDisplayStart, &updateDisplayStart, &exitDisplayStart);
State SettingsStart = State(SETTINGSSTART, &enterSettingsStart, &updateSettingsStart, &exitSettingsStart);
// Settings Timers
State SettingsManualDrive = State(SETTINGSMANUALDRIVE, &enterManualDrive, &updateManualDrive, &exitManualDrive);
State SettingsStabilityTime = State(SETTINGSSTABILITYTIME, &enterSettingsStabilityTime, &updateSettingsStabilityTime, &exitSettingsStabilityTime);
State SettingsSwitchingDelay = State(SETTINGSSWITCHINGDELAY, &enterSwitchingDelay, &updateSwitchingDelay, &exitSwitchingDelay);
State SettingsWarmUpTime = State(SETTINGSWARMUPTIME, &enterWarmUpTime, &updateWarmUpTime, &exitWarmUpTime);
// Settings Calibration
// State SettingsCalibrateZeroVoltage = State(SETTINGSCALIBRATEZEROVOLTAGE, &enterCalibrateZeroVoltage, &updateCalibrateZeroVoltage, &exitCalibrateZeroVoltage);
// State SettingsCalibrateKnownVoltage = State(SETTINGSCALIBRATEKNOWNVOLTAGE, &enterCalibrateKnownVoltage, &updateCalibrateKnownVoltage, &exitCalibrateKnownVoltage);
// Settings Input
State SettingsInput = State(SETTINGSINPUT, &enterSettingsInput, &updateSettingsInput, &exitSettingsInput);


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
            // writer->write((String)"STATE \"" + state->getState() + "\" {5}", "IS UNKNOWN, BUG? {5}");
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
            "VERSION " SW_VERSION,
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
        ">>SETTINGS<< 0/5",
        "Press #- to cycle settings"
    );

}

void updateSettingsStart(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    if (key == '#') {
        // DISABLED - manual drive is not implemented completely
        // dsm->transitionTo(SettingsManualDrive);

        dsm->transitionTo(SettingsManualDrive);
    }

    if (key == '*') {
        
        // TODO since this does not work and we fix it by going to
        // do a full reset, we should just do display SM reset here
        // dsm->deps->sm->immediateTransitionTo(
        //     *dsm->deps->sm->getPreviousState()
        // );

        // We should save new settings but since this is just
        // "enter menu" to settings, there is no need to save

        // After saving, reset board
        // TODO CRIT not tested on real hardware! Do not work 

        // NOTE this needs wire from PIN 6 to RST!
        digitalWrite(6, LOW);

    }

}

void exitSettingsStart(FSM* dsm) {}


/**
 * MANUALDRIVE -STATE
 * 
*/
void enterManualDrive(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "MAN OVERDRVE 1/4",
        "Activate manual overdrive with * or press # to continue to next setting"
    );

}

void updateManualDrive(FSM* dsm) {

    char key = DISPLAY_GET_KEY;
    String key_buffer = "";

    if (key == '#') {
        dsm->transitionTo(SettingsStabilityTime);
    }

    if (key == '*') {
        dsm->transitionTo(SettingsInput);
    }

}

void exitManualDrive(FSM* dsm) {
    // Flush key buffer to avoid old keypresses in other states
    dsm->deps->settings->clear_num_buffer();
}


/**
 * SETTINGSSTABILITYTIME -STATE
 * 
*/
void enterSettingsStabilityTime(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "STABILITY TI 2/4",
        "Change current value \"{3}\" by pressing * or press # to continue to next setting"
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

void exitSettingsStabilityTime(FSM* dsm) {
    // Flush key buffer to avoid old keypresses in other states
    dsm->deps->settings->clear_num_buffer();
}

/**
 * SETTINGSSWITCHINGDELAY -STATE
 * 
*/
void enterSwitchingDelay(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "SWITCH DELAY 3/4",
        "Change current value \"{3}\" by pressing * or press # to continue to next setting"
    );

}

void updateSwitchingDelay(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    dsm->deps->writer->variable(
        SECOND,
        (String)dsm->deps->timer->get_remaining_time(SWITCHING_DELAY)
    );

    if (key == '#') {
        dsm->transitionTo(SettingsWarmUpTime);
    }

    if (key == '*') {
        dsm->transitionTo(SettingsInput);
    }

}

void exitSwitchingDelay(FSM* dsm) {
    // Flush key buffer to avoid old keypresses in other states
    dsm->deps->settings->clear_num_buffer();
}


/**
 * SETTINGSWARMUPTIME -STATE
 * 
*/
void enterWarmUpTime(FSM* dsm) {

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "WARM UP TIME 4/4",
        "Change current value \"{3}\" by pressing * or press # to continue to next setting"
    );

}

void updateWarmUpTime(FSM* dsm) {

    char key = DISPLAY_GET_KEY;

    dsm->deps->writer->variable(
        SECOND,
        (String)dsm->deps->timer->get_remaining_time(WARM_UP_TIME)
    );

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
    // Flush key buffer to avoid old keypresses in other states
    dsm->deps->settings->clear_num_buffer();

    dsm->deps->writer->clear();
    dsm->deps->writer->setMode(RAW);
}


/**
 * SETTINGS CALIBRATE ZERO VOLTAGE
 *
 */
/*
void enterCalibrateZeroVoltage(FSM *dsm)
{

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "CAL 0V LEVEL 5/6",
        "Press * to set 0V calibration of grid, gen, and load sensors. Press # to continue to next setting");
}

void updateCalibrateZeroVoltage(FSM *dsm)
{

    char key = DISPLAY_GET_KEY;

    if (key == '#')
    {
        dsm->transitionTo(SettingsCalibrateKnownVoltage);
    }

    if (key == '*')
    {
        // Calibration requested, calibrate all voltage sensors
        // TODO do not calibrate all sensors, just selected one
        // TODO get calibration func statuses?
        dsm->deps->hardware->calibrateZeroVoltage(PIN_VOLTAGE_GRID);
        dsm->deps->hardware->calibrateZeroVoltage(PIN_VOLTAGE_GENERATOR);
        dsm->deps->hardware->calibrateZeroVoltage(PIN_VOLTAGE_LOAD);
        // TODO give some feedback that calibration was done
    }
}

void exitCalibrateZeroVoltage(FSM *dsm) {}
*/
/**
 * SETTINGS CALIBRATE KNOWN VOLTAGE
 *
 */
/*
void enterCalibrateKnownVoltage(FSM *dsm)
{

    dsm->deps->writer->clear();
    dsm->deps->writer->write(
        "CAL VOLTAGE  6/6",
        "Press * to set known voltage for calibration of grid, gen, and load sensors. Press # to continue to next setting");
}

void updateCalibrateKnownVoltage(FSM *dsm)
{

    // TODO CRIT know we are calibrating all voltage sensors. That means that input value (current, known voltage)
    // is used to calibrate all sensors. This is not good. We should calibrate only one sensor at the time and ask
    // user which sensor to calibrate. This is because we have different calibration values for different sensors.

    char key = DISPLAY_GET_KEY;

    if (key == '#')
    {
        dsm->transitionTo(DisplayUnknownStart);
    }

    if (key == '*')
    {
        // Calibration requested, calibrate all voltage sensors but
        // before that, get known voltage from user
        dsm->transitionTo(SettingsInput);
    }
}

void exitCalibrateKnownVoltage(FSM *dsm) {}
*/

/**
 * SETTINGSINPUT -STATE
 * 
*/
void enterSettingsInput(FSM* dsm) {

    dsm->deps->settings->init_num_buffer();
    dsm->deps->settings->clear_num_buffer();

    switch (dsm->getPreviousState()->getState()) {
        case SETTINGSMANUALDRIVE:

            // Get saved source from eeprom
            dsm->deps->settings->add_num_buffer(
                dsm->deps->hardware->eepromRead(
                    EEPROM_ADDRESS_SOURCE
                )
            );

            // TODO CRIT there should be third option for manual input
            // source, 3 = AUTO
            // NOTE no need to implement if manual drive is "blocking" action
            dsm->deps->writer->write(
                "Type new value for instant manual source or press * to cancel back to AUTO. 1 = GRID and 2 = GENERATOR",
                "{3}"
            );
            break;

        case SETTINGSSTABILITYTIME:

            // Get current stability time from timer
            dsm->deps->settings->add_big_num_buffer(
                dsm->deps->timer->get_initial_time(STABILITY_TIME)  // Gets the init time without down ticked time
            );
            
            // Draw template
            dsm->deps->writer->write(
                "Type new value for stability time and press # to save or press * to cancel without saving",
                "{3}"
            );
            break;

        case SETTINGSSWITCHINGDELAY:

            dsm->deps->settings->add_big_num_buffer(
                dsm->deps->timer->get_initial_time(SWITCHING_DELAY)
            );

            dsm->deps->writer->write(
                "Type new value for switching delay and press # to save or press * to cancel without saving",
                "{3}"
            );
            break;
            
        case SETTINGSWARMUPTIME:

            dsm->deps->settings->add_big_num_buffer(
                dsm->deps->timer->get_initial_time(WARM_UP_TIME)
            );

            dsm->deps->writer->write(
                "Type new value for warm up time and press # to save or press * to cancel without saving",
                "{3}"
            );
            break;

        case SETTINGSCALIBRATEKNOWNVOLTAGE:

            dsm->deps->settings->add_big_num_buffer(
                240
            );

            dsm->deps->writer->write(
                "Type new value for known voltage and press # to save or press * to cancel without saving",
                "{3}"
            );
            
            break;

        default:
            dsm->transitionTo(DisplayStart);
            break;
        }
}

void updateSettingsInput(FSM *dsm) {

    char key = DISPLAY_GET_KEY;
    String key_buffer = dsm->deps->settings->get_num_buffer_string();
    String current_input = "";

    // Conditional template drawing
    switch (dsm->getPreviousState()->getState()) {

        case SETTINGSMANUALDRIVE:
            switch (dsm->deps->settings->get_num_buffer()->buffer[0]) {
                case 1:
                    current_input = "GRID";
                    break;
                case 2:
                    current_input = "GENERATOR";
                    break;
                case 3:
                    current_input = "AUTO";
                    break;
                default:
                    current_input = "UNKNOWN";
                    break;
            }

            dsm->deps->writer->variable(
                SECOND,
                // TODO ish? String "GRID" is longer than its template definition on
                // enterSettingsInput. This works still just fine...
                current_input
            );
            break;

        case SETTINGSSTABILITYTIME:
            // Just draw the number. On enter we draw the template and get the initial value
            dsm->deps->writer->variable(
                SECOND,
                dsm->deps->settings->get_num_buffer_string()
            );
            break;

        case SETTINGSSWITCHINGDELAY:
            dsm->deps->writer->variable(
                SECOND,
                dsm->deps->settings->get_num_buffer_string()
            );
            break;

        case SETTINGSWARMUPTIME:
            dsm->deps->writer->variable(
                SECOND,
                dsm->deps->settings->get_num_buffer_string()
            );
            break;

        case SETTINGSCALIBRATEKNOWNVOLTAGE:
            dsm->deps->writer->variable(
                SECOND,
                dsm->deps->settings->get_num_buffer_string());
            break;

        default:
            break;

    }
    

    // Pressed some key
    if (key_is_number(key) == true) {

        // Stupid casting to get integer from the keypad
        int number = key - '0';

        switch (dsm->getPreviousState()->getState()) {

            case SETTINGSMANUALDRIVE:

                // Since we are getting input for manual drive and it is
                // represented with one digit, we allow one digit to be inputted
                // at a time.
                //
                if (dsm->deps->settings->get_num_buffer()->length != 0) {
                    // Since there was already a digit(s), discard them and save
                    // the new one to be possible input. (User can toggle between
                    // 1 and 2)
                    dsm->deps->settings->clear_num_buffer();
                }
                dsm->deps->settings->add_num_buffer(number);

                break;

            case SETTINGSSTABILITYTIME:
            case SETTINGSSWITCHINGDELAY:
            case SETTINGSWARMUPTIME:

                // Since we are getting input for stability time and it is
                // represented with two digits, we allow two digits to be inputted
                // at a time.
                //
                if (dsm->deps->settings->get_num_buffer()->length >= 2) {
                    // Since there was already two digits, discard them and save
                    // the new one to be possible input. (User can toggle between
                    // 1 and 2 but when the user press # - it'll be saved)
                    dsm->deps->settings->clear_num_buffer();
                }
                dsm->deps->settings->add_num_buffer(number);

                break;

            case SETTINGSCALIBRATEKNOWNVOLTAGE:

                // Since we are getting input for known voltage and it is
                // represented with three digits, we allow three digits to be inputted
                // at a time.
                
                if (dsm->deps->settings->get_num_buffer()->length >= 3)
                {
                    // Since there was already three digits, discard them and save
                    // the new one to be possible input.
                    dsm->deps->settings->clear_num_buffer();
                }
                dsm->deps->settings->add_num_buffer(number);

                break;

            default:
                break;
        }
    }

    if (key == '#') {

        switch (dsm->getPreviousState()->getState()) {

            case SETTINGSMANUALDRIVE:
                // Doing nothing since we do not allow manual drive to be active "in background".
                // When setting page is active, only then override source can be used. When navigating
                // away from settings page, override source is set to AUTO.
                break;
                
            case SETTINGSSTABILITYTIME:
                dsm->deps->timer->set_timer(
                    STABILITY_TIME,
                    dsm->deps->settings->get_num_buffer_int()
                );
                break;

            case SETTINGSSWITCHINGDELAY:
                dsm->deps->timer->set_timer(
                    SWITCHING_DELAY,
                    dsm->deps->settings->get_num_buffer_int()
                );
                break;

            case SETTINGSWARMUPTIME:
                dsm->deps->timer->set_timer(
                    WARM_UP_TIME,
                    dsm->deps->settings->get_num_buffer_int()
                );
                break;

            case SETTINGSCALIBRATEKNOWNVOLTAGE:
                // Calibrate all voltage sensors to the value inputted by the user

                // Grid
                dsm->deps->hardware->calibrateKnownVoltage(
                    PIN_VOLTAGE_GRID,
                    dsm->deps->settings->get_num_buffer_int()
                );
                // Generator
                dsm->deps->hardware->calibrateKnownVoltage(
                    PIN_VOLTAGE_GENERATOR,
                    dsm->deps->settings->get_num_buffer_int()
                );
                // Load
                dsm->deps->hardware->calibrateKnownVoltage(
                    PIN_VOLTAGE_LOAD,
                    dsm->deps->settings->get_num_buffer_int()
                );

                break;

            default:
                break;
        }

        dsm->deps->settings->commit_setting(
            dsm->deps->hardware,
            dsm->getPreviousState()->getState(),
            dsm->deps->settings->get_num_buffer_int()
        );

        // TODO this is not optimal place to switch contactors
        // TODO CRIT and therefore this has been disabled for now
        // if (s == GRID) {
        //     dsm->deps->hardware->setDigital(PIN_CONTACTOR_GENERATOR, false);
        //     dsm->deps->hardware->wait(100);
        //     dsm->deps->hardware->setDigital(PIN_CONTACTOR_GRID, true);
        // } else if (s == GENERATOR) {
        //     dsm->deps->hardware->setDigital(PIN_CONTACTOR_GRID, false);
        //     dsm->deps->hardware->wait(100);
        //     dsm->deps->hardware->setDigital(PIN_CONTACTOR_GENERATOR, true);
        // }

        dsm->transitionTo(
            *dsm->getPreviousState()
        );
    }

    if (key == '*') {

        // If we were on manual drive setting, we should go back to
        // AUTO mode before exiting settings
        if (dsm->getPreviousState()->getState() == SETTINGSMANUALDRIVE) {
            dsm->deps->settings->set_override_source(AUTOMATIC);
        }

        // Cancel pressed, go back wherever you came from
        // by using getter of previous state.
        dsm->transitionTo(
            *dsm->getPreviousState()
        );
    }

}

void exitSettingsInput(FSM* dsm) {
    dsm->deps->settings->free_num_buffer();

    dsm->deps->writer->clear();
    dsm->deps->writer->setMode(RAW);
}

#endif // UNIT_TEST