#include "hardware.h"


Hardware::Hardware() {}

unsigned long Hardware::time() {
    #ifdef ARDUINO
    // If building for Arduino, use real millis function
    return millis();
    #else
    // -1 indicates that mocking was left off
    return UNKNOWN;
    #endif
}

void Hardware::setDigital(uint8_t pin, uint8_t val) {
    #ifdef ARDUINO
    return digitalWrite(pin, val);
    #endif
}

int Hardware::getDigital(uint8_t pin) {
    #ifdef ARDUINO
    return digitalRead(pin);
    #else
    return UNKNOWN;
    #endif
}

/**
 * Waits using built-in delay function call.
 * Does not wait if native environment.
*/
void Hardware::wait(unsigned long ms) {
    #ifdef ARDUINO
    delay(ms);
    #endif
    int i = 0;
    i++;
}

int Hardware::getVoltageAC(uint8_t pin) {
    #ifdef ARDUINO
    
    if (digitalRead(PIN_IS_SIMULATOR) == 0) {
        /*
            When jumper wire connects D53 to GND
            it can be thought to be simulator.
            Since simulator does not have voltage
            sensor modules, switches are used. 
            0 = switch on
            1 = switch off
        */
       if (digitalRead(pin) == 0) {
            return 250;
       }

       return 0;
    }
    ZMPT101B z = ZMPT101B(pin);
    float v = z.getVoltageAC() * 10;
    return floor(v);
    #else
    return -1;
    #endif
}

// TODO digitalRead
// TODO digitalWrite
// TODO analogRead
