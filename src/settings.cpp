#include "settings.h"


Settings::Settings() {
    this->num_buffer.length = 0;
    this->num_buffer.buffer = NULL;
    this->override_source = AUTOMATIC;
}

// Settings::~Settings() {
//     free(this->num_buffer.buffer);
// }

void Settings::load(Hardware* hardware) {
    
    // Get source from EEPROM and then save it to settings
    int source = hardware->eepromRead(EEPROM_ADDRESS_SOURCE);
    if (source < 0 || source > 3) {
        // TODO found unknown source, what to do?
        source = AUTOMATIC;
    }
    this->override_source = (enum source)source;

}

void Settings::init_num_buffer() {
    this->num_buffer.buffer = (int*)malloc(sizeof(int) * NUM_BUFFER_MAX_LENGTH);
    this->num_buffer.length = 0;

}

void Settings::free_num_buffer() {
    free(this->num_buffer.buffer);
    this->num_buffer.length = 0;
}

void Settings::clear_num_buffer() {
    this->num_buffer.length = 0;
}

void Settings::add_num_buffer(int num) {
    if (this->num_buffer.length >= NUM_BUFFER_MAX_LENGTH) {
        return;
    }
    this->num_buffer.buffer[this->num_buffer.length] = num;
    this->num_buffer.length++;
}

numBuffer* Settings::get_num_buffer() {
    return &this->num_buffer;
}

// When setting override as active grid & genset
// inputs are ignored when selecting for instance
// the input of the load. Can be triggered at least
// from settings (#-key) when selecting manual ATS input.
void Settings::set_override_source(source source) {
    this->override_source = source;
}

overrideSource Settings::get_override() {
    if (this->override_source == GRID || this->override_source == GENERATOR) {
        return {
            .active = true,
            .s = this->override_source
        };
    } else {
        return {
            .active = false,
            .s = AUTOMATIC
        };
    }
}

// TODO CRIT does it need Hardware *hardware as a first parameter?
void Settings::commit_setting(Hardware *hardware, States state, int value)
{
    switch (state) {
        case SETTINGSMANUALDRIVE:
            hardware->eepromWrite(
                EEPROM_ADDRESS_SOURCE,
                value
            );
            break;
        case SETTINGSSTABILITYTIME:
            // TODO CRIT change hardware to timer, and eeprom to timer
            break;
        case SETTINGSSWITCHINGDELAY:

            break;
        case SETTINGSWARMUPTIME:

            break;
        default:
            break;
    }
}

#ifndef UNIT_TEST
String Settings::get_num_buffer_string() {

    String key_buffer = "";
    
    for (int i = 0; i < this->get_num_buffer()->length; i++) {
        key_buffer += this->get_num_buffer()->buffer[i];
    }

    return key_buffer;
}
#else
std::string Settings::get_num_buffer_string() { return ""; }
#endif  //UNIT_TEST
