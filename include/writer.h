#ifndef WRITER_H
#define WRITER_H

#ifndef UNIT_TEST
#include <LiquidCrystal.h>
#include "settings.h"

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

struct Variable {
    String variable;
    bool exists;
    int length;
    int start_index;
    int end_index;
    int initial_length;
};

struct Variables {
    Variable first;
    Variable second;
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
        void write(String row);
        void write(String row, String second_row);
        String variableToRow(writeRow row_select, int characters, String text, String variable);
        void variable(writeRow row, String var);
        int update();
        int set_write_to_row(writeRow new_write_row);
        Variables variables;
};

#endif  // UNIT_TEST
#endif  // WRITER_H