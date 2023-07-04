#include "hardware.h"
#ifndef UNIT_TEST
#include <EEPROM.h>
#endif // UNIT_TEST


Hardware::Hardware()
    : z_grid(PIN_VOLTAGE_GRID),
      z_generator(PIN_VOLTAGE_GENERATOR),
      z_load(PIN_VOLTAGE_LOAD) {}

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

    float v;

    if (pin == PIN_VOLTAGE_GRID) {
        v = this->z_grid.getVoltageAC() * 10;
    } else if (pin == PIN_VOLTAGE_GENERATOR) {
        v = this->z_generator.getVoltageAC() * 10;
    } else if (pin == PIN_VOLTAGE_LOAD) {
        v = this->z_load.getVoltageAC() * 10;
    } else {
        return UNKNOWN;
    }

    return floor(v);

    #else // ARDUINO
    return UNKNOWN;
    #endif // ARDUINO
}

// TODO save known voltage calibration value to EEPROM
int Hardware::calibrateKnownVoltage(uint8_t pin, uint16_t knownVoltage) {

    #ifdef ARDUINO
    if (digitalRead(PIN_IS_SIMULATOR) == 0) {
        return UNKNOWN;
    }

    float expectedAdc = (knownVoltage / VREF) * ADC_SCALE;

    if (pin == PIN_VOLTAGE_GRID) {
        float readAdc = this->z_grid.getVoltageAC();
        float newSensitivity = (readAdc / expectedAdc) * this->z_grid.getSensitivity();
        this->z_grid.setSensitivity(newSensitivity);
    } else if (pin == PIN_VOLTAGE_GENERATOR) {
        float readAdc = this->z_generator.getVoltageAC();
        float newSensitivity = (readAdc / expectedAdc) * this->z_generator.getSensitivity();
        this->z_generator.setSensitivity(newSensitivity);
    } else if (pin == PIN_VOLTAGE_LOAD) {
        float readAdc = this->z_load.getVoltageAC();
        float newSensitivity = (readAdc / expectedAdc) * this->z_load.getSensitivity();
        this->z_load.setSensitivity(newSensitivity);
    } else {
        return UNKNOWN;
    }

    return 0;

    #else // ARDUINO
    return UNKNOWN;
    #endif // ARDUINO
}

// TODO save zero point to EEPROM
int Hardware::calibrateZeroVoltage(uint8_t pin) {
    #ifdef ARDUINO
    if (digitalRead(PIN_IS_SIMULATOR) == 0) {
        return UNKNOWN;
    }

    int zeroPoint;

    if (pin == PIN_VOLTAGE_GRID) {
        zeroPoint = this->z_grid.calibrate();
        this->z_grid.setZeroPoint(zeroPoint);
    } else if (pin == PIN_VOLTAGE_GENERATOR) {
        zeroPoint = this->z_generator.calibrate();
        this->z_generator.setZeroPoint(zeroPoint);
    } else if (pin == PIN_VOLTAGE_LOAD) {
        zeroPoint = this->z_load.calibrate();
        this->z_load.setZeroPoint(zeroPoint);
    } else {
        return UNKNOWN;
    }
    #endif // ARDUINO
}

bool Hardware::isSimulator() {
    #ifdef ARDUINO
    if (digitalRead(PIN_IS_SIMULATOR) == 0) {
        return true;
    }
    return false;
    #else
    return false;
    #endif  // ARDUINO

}

int Hardware::initEEPROM() {
    #ifdef ARDUINO
        // Check if EEPROM has been initialized
        if (EEPROM.read(EEPROM_INIT_ADDRESS) != EEPROM_INITIALIZED) {

            Serial.println("Initializing EEPROM");

            // By default, source is AUTO (3), not grid (1) or generator (2)
            EEPROM.write(EEPROM_ADDRESS_SOURCE, 3);

            // By default, stability is 30 seconds, warmup 30 and transfer 5
            EEPROM.write(EEPROM_ADDRESS_STABILITY_TIME, 30);
            EEPROM.write(EEPROM_ADDRESS_WARM_UP_TIME, 30);
            EEPROM.write(EEPROM_ADDRESS_SWITCHING_DELAY_TIME, 5);
            
            // Mark EEPROM as initialized
            EEPROM.write(EEPROM_INIT_ADDRESS, EEPROM_INITIALIZED);
        }
        return 0;
    #else
        return 568;
    #endif
}

int Hardware::eepromRead(int address) {
    #ifdef ARDUINO
    return EEPROM.read(address);
    #else
    return 568;
    #endif
}

int Hardware::eepromWrite(int address, int value) {
    #ifdef ARDUINO
    EEPROM.write(address, value);
    return 0;
    #else
    return 568;
    #endif
}

// TODO digitalRead
// TODO digitalWrite
// TODO analogRead
