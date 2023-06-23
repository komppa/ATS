#include "logger.h"


void logMessage(const char* level, const char* message) {
    unsigned long timestamp = millis();
    Serial.print("[");
    Serial.print(timestamp);
    Serial.print("] [");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
}