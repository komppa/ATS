#include "external_time.h"
// #ifdef ARDUINO
// #include <Arduino.h>
// #endif


ExternalTime::ExternalTime() {
    this->time = 0;
};

int ExternalTime::get_time() {
    this->time = 123;
    return this->time;
}

