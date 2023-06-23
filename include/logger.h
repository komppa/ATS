#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>


enum LogLevel {NONE, ERROR, WARN, INFO, DEBUG};

// Set current log level here!
#define CURRENT_LOG_LEVEL DEBUG


// Macro for logging with log level and timestamp
#define LOG(level, message) if (level <= CURRENT_LOG_LEVEL) logMessage(#level, message)

void logMessage(const char* level, const char* message);



#endif // LOGGER_H