#ifndef LOGGER_H
#define LOGGER_H

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <iostream>
#include <chrono>
#endif // UNIT_TEST
#include "hardware.h"


enum LogLevel {NONE, ERROR, WARN, INFO, DEBUG};

// Set current log level here!
#define CURRENT_LOG_LEVEL DEBUG


// Macro for logging with log level and timestamp
#define LOG(level, message) if (level <= CURRENT_LOG_LEVEL) logger.log(#level, message)


class Logger {
    private:
        Hardware *hardware;
    public:
        Logger(Hardware*);
        virtual void log(const char*, const char*);
};


#endif // LOGGER_H