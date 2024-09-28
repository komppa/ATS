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

        // In order to get log level and timestamp in tab separated format,
        // we need to calculate the length of the timestamp and add spaces accordingly.
        auto timestamp = this->hardware->time();
        
        // Get how many digits integer timestamp has
        int digits = 0;
        if (timestamp == 0) {
            digits = 1;
        } else {
            for (unsigned long i = timestamp; i > 0; i /= 10) {
                digits++;
            }
        }


        Serial.print("[");
        Serial.print(timestamp);
        Serial.print("]");
        for (int i = 0; i < 10 - digits; i++) {
            Serial.print(" ");
        }
        Serial.print("[");
        Serial.print(level);
        Serial.print("]  ");
        Serial.println(message);
    #endif
}
