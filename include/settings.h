#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
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


class Settings {
    private:
        numBuffer num_buffer;
    public:
        Settings();
        // ~Settings();
        void clear_num_buffer();
        void add_num_buffer(int num);
        numBuffer* get_num_buffer();
        #ifndef UNIT_TEST
        String get_num_buffer_string();
        #else
        std::string get_num_buffer_string();
        #endif  // UNIT_TEST
        // void commit_num_buffer(States state, int* num_buffer);
};


#endif // SETTINGS_H