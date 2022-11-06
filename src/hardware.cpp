#include "hardware.h"


Hardware::Hardware() {}

unsigned long Hardware::millis() {
    #ifdef ARDUINO
    // If building for Arduino, use real millis function
    return millis();
    #else
    // -1 indicates that mocking was left off
    return UNKNOWN;
    #endif
}

void Hardware::digitalWrite(uint8_t pin, uint8_t val) {
    #ifdef ARDUINO
    return digitalWrite(pin, val);
    #endif
}

int Hardware::digitalRead(uint8_t pin) {
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
void Hardware::delay(unsigned long ms) {
    #ifdef ARDUINO
    // delay(ms);
    #endif
    int i = 0;
    i++;
}

int Hardware::getVoltageAC(uint8_t pin) {
    #ifdef ARDUINO
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
