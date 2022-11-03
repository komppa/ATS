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

// TODO digitalRead
// TODO digitalWrite
// TODO analogRead
