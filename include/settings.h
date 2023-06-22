#ifndef SETTINGS_H
#define SETTINGS_H

#include "estates.h"
#include <stdlib.h>
#include "hardware.h"
#include "timers.hpp"
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif  // UNIT_TEST

#define SETTING_MIN_VOLTAGE_THRESHOLD   50
#define SW_VERSION                      0.10

#define NUM_BUFFER_MAX_LENGTH           10

// Icons
#define ICON_GRID                       0
#define ICON_GENERATOR                  1
#define ICON_LOAD                       2

 
struct numBuffer {
    int length;
    int* buffer;
};

enum source {
    GRID = 1,
    GENERATOR = 2,
    AUTOMATIC = 3
};

struct overrideSource {
    bool active;
    source s;
};


class Settings {
    private:
        numBuffer num_buffer;
        source override_source;
    public:
        Settings();
        void load(Hardware*, Timer*);
        // ~Settings();
        void init_num_buffer();
        void free_num_buffer();
        void clear_num_buffer();
        void add_num_buffer(int num);
        void add_big_num_buffer(int);
        int get_num_buffer_int();
        numBuffer* get_num_buffer();
        void set_override_source(source);
        overrideSource get_override();
        void commit_setting(Hardware* hardware, States state, uint8_t value);
        #ifndef UNIT_TEST
        String get_num_buffer_string();
        #else
        std::string get_num_buffer_string();
        #endif  // UNIT_TEST
};


#endif // SETTINGS_H