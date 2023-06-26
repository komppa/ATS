#include "logger.h"


Logger::Logger(Hardware *hardware) {
    this->hardware = hardware;
}

void Logger::log(const char* level, const char* message) {
    #ifdef UNIT_TEST
        auto millis = this->hardware->time();
        std::cout << "[" << millis << "] [" << level << "] " << message << std::endl;
    #else
        // Code for compiling with Arduino IDE
        auto timestamp = this->hardware->time();
        Serial.print("[");
        Serial.print(timestamp);
        Serial.print("] [");
        Serial.print(level);
        Serial.print("] ");
        Serial.println(message);
    #endif
}
