#include "sound.h"


void startup_sound() {

    #ifndef UNIT_TEST
    tone(PIN_BUZZER, 800, 75);
    delay(75);
    tone(PIN_BUZZER, 1000, 75);
    delay(75);
    tone(PIN_BUZZER, 1200, 75);
    delay(75);
    tone(PIN_BUZZER, 1400, 75);
    delay(75);
    tone(PIN_BUZZER, 1200, 75);
    delay(75);
    tone(PIN_BUZZER, 1000, 75);
    delay(75);
    tone(PIN_BUZZER, 800, 150); 
    delay(150);
    noTone(PIN_BUZZER);
    #endif  // UNIT_TEST
}