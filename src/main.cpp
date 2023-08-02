/**
   Automatic Transfer Switch

   Copyright (C) 2020, Konsta Rantakangas.
   Released under the MIT License.
*/
#ifndef UNIT_TEST
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Arduino.h>
#include "writer.h"
#include "hardware.h"
#include <stdio.h>
#include "FiniteStateMachine.hpp"   // for Deps
#include "display.h"
#include "settings.h"
#include "logger.h"


// Display contacts: VSS=GND, VDD=5V, RW=GND, A=5V(VIA 220OHM RESISTOR), K=GND 
//                RS, E , D4,D5,D6,D7
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
Writer writer = Writer(&lcd);

Hardware hardware;
Timer timer(&hardware);
Settings settings;
Logger logger(&hardware);

Deps ATSFSMDeps = {
    .hardware = &hardware,
    .timer = &timer,
    .keypad = &keypad,
    .writer = &writer
};

extern State UnknownStart;
FSM sm = FSM(UnknownStart, &ATSFSMDeps);

Deps displayFSMDeps = {
    .hardware = &hardware,
    .timer = &timer,
    .keypad = &keypad,
    .writer = &writer,
    .sm = &sm,
    .settings = &settings
};

extern State DisplayUnknownStart;
FSM display = FSM(DisplayUnknownStart, &displayFSMDeps);

byte generator_icon[8] = {B00000, B11111, B10011, B11111, B10101, B11111, B10011};
byte load_icon[8] = {B00000, B00100, B01110, B11111, B11111, B11011, B11011};
byte grid_icon[8] = {B10001, B11111, B10001, B10001, B10001, B10001, B10001, B10001};


void setup() {

    Serial.begin(115200);

    LOG(INFO, "===============================");
    LOG(INFO, " ATS - Automatic Transfer Switch");
    LOG(INFO, "       Version: " SW_VERSION);
    LOG(INFO, "   Booting Up... Please Wait.");
    LOG(INFO, "    Crafted with precision by");
    LOG(INFO, "           K&I ENT.");
    LOG(INFO, "===============================");
    LOG(INFO, "");

    LOG(DEBUG, "Initializing LCD");
    lcd.begin(DISPLAY_WIDTH, 2);
    LOG(DEBUG, "Initialized LCD");

    lcd.createChar(ICON_GRID, grid_icon);
    lcd.createChar(ICON_GENERATOR, generator_icon);
    lcd.createChar(ICON_LOAD, load_icon);

    lcd.clear();
    lcd.setCursor(0, 0);

    writer.setMode(RAW);

    // For checking whether this is simulator in runtime
    pinMode(PIN_IS_SIMULATOR, INPUT_PULLUP);

    pinMode(PIN_CONTACTOR_GRID, OUTPUT);
    pinMode(PIN_CONTACTOR_GENERATOR, OUTPUT);

    pinMode(PIN_VOLTAGE_GRID, INPUT_PULLUP);
    pinMode(PIN_VOLTAGE_GENERATOR, INPUT_PULLUP);

    // Buzzer
    pinMode(PIN_BUZZER, OUTPUT);

    // Init EEPROM
    LOG(DEBUG, "Initializing HW EEPROM");
    hardware.initEEPROM();
    LOG(DEBUG, "Initialized EEPROM & loading settings");

    // Get values from EEPROM at startup
    settings.load(&hardware, &timer);
    LOG(DEBUG, "Loaded settings from EEPROM");

    // Startup sound
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

    // Wait a little bit for hardware to settle
    delay(500);

}


void loop() {

    // Limiter
    delay(100);

    if (settings.get_override().active) {
        LOG(DEBUG, "Override mode active");
        // Display the override message on the LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MANUAL-SRC:GRID");
        lcd.setCursor(0, 1);
        lcd.print("EXIT MAN:PRS KEY");
        
        // Wait for key press to deactivate override mode
        char key = keypad.getKey();
        if (key) {
            // Deactivate override mode by saying the source is automatic
            settings.set_override_source(AUTOMATIC);
            lcd.clear();
        }
        
        // Return early so the FSM is not updated while in override mode
        return;
    }

    // Regular updates
    timer.update();
    writer.update();
    if (millis() > 1000) {
        sm.update();
        display.update();
    }
}


#endif // Do not compile setup and loop if unit testing