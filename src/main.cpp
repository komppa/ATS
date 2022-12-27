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


auto writer = new Writer(&lcd);


void updateCursor() {
    if (millis() / 250 % 2 == 0 ) {
        lcd.cursor();
    } else {
        lcd.noCursor();
    }
}


void setup() {
    Serial.begin(115200);
    lcd.begin(DISPLAY_WIDTH, 2);

    lcd.clear();
    lcd.setCursor(0, 0);

    writer->setMode(MIDDLE);
    writer->write("SELET BY PRESSING # -BUTTON", "123456789abcdefghijklmn");

}




int period = 1000;
unsigned long time_now = 0;

void loop() {


    writer->update();

    char key = keypad.getKey();

    if (key) {
        if (key == '#') {
            writer->write("> SETTINGS <", "You pressed key #");
        }
    }

    if ((unsigned long)(millis() - time_now) > period) {
        time_now = millis();
    }
  
}


#endif // Do not compile setup and loop if unit testing