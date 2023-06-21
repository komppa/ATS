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
    lcd.begin(DISPLAY_WIDTH, 2);

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

    // Init EEPROM
    hardware.initEEPROM();

    // Wait a little bit for hardware to settle
    // delay(500);

}


void loop() {

    // Limitter
    delay(100);

    // Updates
    sm.update();
    timer.update();
    writer.update();
    // TODO wait few seconds before starting screen refreshing on startup
    // to ensure that electricity can be flowed from grid as fast as possible.
    if (millis() > 1000) display.update();

}


#endif // Do not compile setup and loop if unit testing