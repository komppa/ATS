#ifndef HARDWARE_H
#define HARDWARE_H

#include "gmock/gmock.h"
#include <stdint.h>

// If building release for Arduino, include it to be used by this class
#ifdef ARDUINO
#include <Arduino.h>
#endif

#define UNKNOWN -1
#define MOCK_MILLIS(ret_val) (ON_CALL(hardware, millis()).WillByDefault(Return(ret_val)))



class Hardware {
    private:
        int my_var;
    public:
        Hardware();
        virtual unsigned long millis();
        virtual void digitalWrite(uint8_t pin, uint8_t val);
        virtual int digitalRead(uint8_t pin);
};


class MockHardware: public Hardware {
    public:
        MOCK_METHOD(unsigned long, millis, ());
        MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
        MOCK_METHOD(int, digitalRead, (uint8_t));
};

#endif