#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include "ZMPT101B.h"

// If building release for Arduino, include it to be used by this class
#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#ifdef UNIT_TEST
#include "gmock/gmock.h"
#endif

#define UNKNOWN -1
#define MOCK_VOLTAGE(pin, ret_val) (ON_CALL(hardware, getVoltageAC(pin)).WillByDefault(Return(ret_val)))

#define MOCK_MILLIS(ret_val) (ON_CALL(hardware, millis()).WillByDefault(Return(ret_val)))
#define MOCK_FAST_MILLIS(timer, ff_val)                        \
    for (int i = 1; i <= ff_val; i++) {                             \
        if (i % 1000 == 0) {                                        \
            ON_CALL(hardware, millis()).WillByDefault(Return(i));   \
            timer.update();                                         \
        }                                                           \
    }

// Pin definitions
// Contactors
#define PIN_CONTACTOR_GRID      22  // @2560
#define PIN_CONTACTOR_GENERATOR 23  // @2560
// Voltage meas modules
#define PIN_VOLTAGE_GRID        67  // A13 @2560
#define PIN_VOLTAGE_GENERATOR   68  // A14 @2560
#define PIN_VOLTAGE_LOAD        69  // A15 @2560
// Keypad
#define PIN_KEYPAD_ROW_1        5
#define PIN_KEYPAD_ROW_2        4
#define PIN_KEYPAD_ROW_3        3
#define PIN_KEYPAD_ROW_4        2
#define PIN_KEYPAD_COLUMN_1     17  // A3
#define PIN_KEYPAD_COLUMN_2     16  // A2
#define PIN_KEYPAD_COLUMN_3     15  // A1
// Display
#define PIN_DISPLAY_RS          12
#define PIN_DISPLAY_E           11
#define PIN_DISPLAY_D4          10
#define PIN_DISPLAY_D3          9
#define PIN_DISPLAY_D2          8
#define PIN_DISPLAY_D1          7


class Hardware {
    private:
        int my_var;
    public:
        Hardware();
        // Arduino ones
        virtual unsigned long millis();
        virtual void digitalWrite(uint8_t pin, uint8_t val);
        virtual int digitalRead(uint8_t pin);
        virtual void delay(unsigned long ms);
        // Voltage sensor to be able to mocked
        virtual int getVoltageAC(uint8_t pin);
};

#ifdef UNIT_TEST
class MockHardware: public Hardware {
    public:
        // Arduino
        MOCK_METHOD(unsigned long, millis, ());
        MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
        MOCK_METHOD(int, digitalRead, (uint8_t));
        MOCK_METHOD(void, delay, (unsigned long));
        // Voltage sensor
        MOCK_METHOD(int, getVoltageAC, (uint8_t));
};
#endif  // UNIT_TEST

#endif