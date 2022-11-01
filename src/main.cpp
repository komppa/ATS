/**
   Automatic Transfer Switch

   Copyright (C) 2020, Konsta Rantakangas.
   Released under the MIT License.
*/
#ifndef UNIT_TEST
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Arduino.h>
// #include "FiniteStateMachine.h"
// #include "Timers.h"

#define DISPLAY_WIDTH 16
#define DISPLAY_UPDATE_INTERVAL 200

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


bool isGridUp() {
  return analogRead(A4) == 0 ? true : false;
}

bool isGeneratorUp() {
  return analogRead(A5) == 0 ? true : false;
}


enum writeMode {
  RAW, // Disable all stylings
  LEFT,
  MIDDLE
};

enum writeRow {
  AUTO,
  FIRST,
  SECOND
};

class Writer {
  private:
    writeMode mode;
    String row;
    String second_row;
    signed int row_index;
    signed int second_row_index;
    bool row_roll;
    bool second_row_roll;
    unsigned long last_update;
    writeRow write_to_row;
  
  public:
    Writer();
    writeMode getMode();
    setMode(writeMode new_mode);
    write(String row, String second_row);
    update();
    set_write_to_row(writeRow new_write_row);
};

Writer::update() {

  if (this->row_roll == false && this->second_row_roll == false) {
    return;
  }


  if ((this->last_update + DISPLAY_UPDATE_INTERVAL) <= millis()) {

    lcd.clear();
  
    // Does the first row need a roll
    // TODO implement

    // Does the first row need rolling
    if (this->row.length() > DISPLAY_WIDTH) {
      // "Bounce" protection
      // if ((this->last_update + 250) <= millis()) {

      if (this->row_index == (signed int)((-1) * this->row.length())) {
        this->row_index = DISPLAY_WIDTH - 1;
      }

      this->row_index--;

      if (this->row_index > 0) {
        lcd.setCursor(this->row_index, 0);
        // Substringin that the string wont appear to the second row
        lcd.print(this->row.substring(0, (-1) * (this->row_index - DISPLAY_WIDTH)));
      } else {
        // Row should be negative value but since display does not support
        // negative values, it must be substringed on the fly
        lcd.setCursor(0, 0);
        lcd.print(this->row.substring((-1) * this->row_index));
      }

    } else {
      // Scrolling not required for the first row.
      // Still, redraw everything for the first row since
      // everything is cleared on screen on every update.
      lcd.setCursor(floor((DISPLAY_WIDTH - row.length()) / 2 ), 0);
      lcd.print(this->row);
    }

    // Does the second row need rolling
    if (this->second_row.length() > DISPLAY_WIDTH) {


      if (this->second_row_index == (signed int)((-1) * this->second_row.length())) {
        this->second_row_index = DISPLAY_WIDTH - 1;
      }

      this->second_row_index--;

      if (this->second_row_index > 0) {
        Serial.println(">");
        lcd.setCursor(this->second_row_index, 1);
        // Substringin that the string wont appear to the second row
        //lcd.print(this->second_row.substring(0, (-1) * (this->second_row.length() - DISPLAY_WIDTH)));
        lcd.print(this->second_row.substring(0, (-1) * (this->second_row_index - DISPLAY_WIDTH)));
      } else {
        Serial.println("else");
        // Row should be negative value but since display does not support
        // negative values, it must be substringed on the fly
        lcd.setCursor(0, 1);
        lcd.print(this->second_row.substring((-1) * this->second_row_index));
      }
      

    } else {
      // Scrolling not required for the second row.
      // Still, redraw everything for the second row since
      // everything is cleared on screen on every update.
      lcd.setCursor(floor((DISPLAY_WIDTH - second_row.length()) / 2 ), 1);
      lcd.print(this->second_row);
    }

    this->last_update = millis();

  }

}

Writer::Writer() {
  // By default,  all writing styles are disabled
  this->mode = RAW;
  this->row = "";
  this->second_row = "";
  this->row_index = 0;
  this->second_row_index = 0;
  this->row_roll = false;
  this->second_row_roll = false;
  this->last_update = 0;
  this->write_to_row = AUTO;
}

writeMode Writer::getMode() {
  return (writeMode)this->mode;
}

Writer::setMode(writeMode new_mode) {
  this->mode = new_mode;
}


Writer::write(String row, String second_row = "") {

  lcd.clear();
  
  // Index stores which index is used to start
  // writing text on screen for each row.
  // If text is longer than the screen of the device,
  // it is not reasonable to align text center of the screen
  // that is why index is then set to be zero.
  // First row
  signed int index = floor((DISPLAY_WIDTH - row.length()) / 2 );
  if (index > DISPLAY_WIDTH) index = DISPLAY_WIDTH;
  // Second row
  signed int index_second = floor((DISPLAY_WIDTH - second_row.length()) / 2 );
  if (index_second > DISPLAY_WIDTH) index_second = DISPLAY_WIDTH;
  
  
  switch (this->mode) {

    // Just write start of the screen
    case RAW:
      lcd.setCursor(0,0);
      lcd.print(row);

      // Check whether there is the second row to be writed
      if (second_row.length() != 0) {
        lcd.setCursor(0,1);
        lcd.print(second_row);
        this->row = row;
      }

      break;
      
    // Text align center
    case MIDDLE:
      lcd.setCursor(
        index,
        0
      );
      lcd.print(row);
      // Save writed text
      this->row = row;
      // Save the starting postion for the first row text
      this->row_index = index;

      // Check whether there is the second row to be writed
      if (second_row.length() != 0) {
        lcd.setCursor(
          index_second,
          1
        );
        lcd.print(second_row);
        // Save the second row text
        this->second_row = second_row;
        // Save the second row staring index
        this->second_row_index = index_second;
      }

      break;

    // On default, do nothing
    default:
      break;

  }

  // Mark if text in some row is longer than the display is
  if (row.length() > DISPLAY_WIDTH) {
    this->row_roll = true;
  } else {
    this->row_roll = false;
  }
  
  if (second_row.length() > DISPLAY_WIDTH) {
    this->second_row_roll = true;
  } else {
    this->second_row_roll = false;
  }
  
}


Writer::set_write_to_row(writeRow new_write_row) {
  Serial.println("@set_write_to_row");
}


auto writer = new Writer();


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
  writer->write("SELET BY PRESSING # -BUTTON 12345", "123456789abcdefghijklmn");
  
  if (isGridUp() && !isGeneratorUp()) {
    writer->write(
      "GRID IS UP 240V",
      "GENERATOR IS DOWN"
    );
  } else if (isGridUp() && isGeneratorUp()) {
    writer->write(
      "GRID IS UP 240V",
      "GENERATOR IS UP 218V"
    );
  } else if (!isGridUp() && isGeneratorUp()) {
    writer->write(
      "GRID IS DOWN",
      "GENERATOR IS UP 218V"
    );
  } else if (!isGridUp() && !isGeneratorUp()) {
    writer->write(
      "GRID IS DOWN",
      "GENERATOR IS DOWN"
    );
  }


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
