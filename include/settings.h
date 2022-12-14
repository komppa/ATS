#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif  // UNIT_TEST
#include "hardware.h"


#define SETTING_MIN_VOLTAGE_THRESHOLD   50
#define SW_VERSION                      0.10

#define NUM_BUFFER_MAX_LENGTH           10

// Icons
#define ICON_GRID                       0
#define ICON_GENERATOR                  1
#define ICON_LOAD                       2

 
enum States {

	// ATS FSM states
	UNKNOWNSTART,
	NORMAL,
	NORMAL2,
	STABILITY,
	WAITGEN,
	WARMUP,
	SWITCHDELAYTOGEN,
	SWITCHTOGEN,
	SWITCHDELAYTOGRID,
	DETACHGEN,

	// Display FSM states
	DISPLAYUNKNOWNSTART,
	DISPLAYSTART,
	SETTINGSSTART,
	SETTINGSMANUALDRIVE,
	SETTINGSSTABILITYTIME,
	SETTINGSSWITCHINGDELAY,
	SETTINGSWARMUPTIME,
	SETTINGSINPUT

	// TODO Writer states
	// ....

};

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
        void commit_setting(Hardware *hardware, States state, String buffer);
        String get_num_buffer_string();
        #else
        void commit_setting(States state, std::string buffer);
        std::string get_num_buffer_string();
        #endif  // UNIT_TEST
};


#endif // SETTINGS_H