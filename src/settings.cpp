#include "settings.h"


Settings::Settings() {
    this->num_buffer.length = 0;
    this->num_buffer.buffer = (int*)malloc(sizeof(int) * NUM_BUFFER_MAX_LENGTH);
}

// Settings::~Settings() {
//     free(this->num_buffer.buffer);
// }

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

#ifndef UNIT_TEST
// TODO CRIT does it need Hardware *hardware as a first parameter?
// void Settings::commit_setting(States state, String buffer)
void Settings::commit_setting(States state, String buffer)
#else
void commit_setting(States state, std::string buffer)
#endif
{
    switch (state) {
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
