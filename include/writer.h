#ifndef WRITER_H
#define WRITER_H

#ifndef UNIT_TEST
#include <Arduino.h>
#include <LiquidCrystal.h>


#define DISPLAY_WIDTH 16
#define DISPLAY_UPDATE_INTERVAL 200


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
        LiquidCrystal *lcd;
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
        Writer(LiquidCrystal *lcd);
        writeMode getMode();
        int setMode(writeMode new_mode);
        int clear();
        int write(String row);
        int write(String row, String second_row);
        int update();
        int set_write_to_row(writeRow new_write_row);
};

#endif  // UNIT_TEST
#endif  // WRITER_H