#include "external_time.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif

unsigned long get_time() {
#ifndef ARDUINO
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
#else
    return millis();
#endif
}