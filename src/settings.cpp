#include "settings.h"


Settings::Settings() {
    this->num_buffer.length = 0;
    this->num_buffer.buffer = NULL;
    this->override_source = AUTOMATIC;
}

// Settings::~Settings() {
//     free(this->num_buffer.buffer);
// }

void Settings::load(Hardware* hardware, Timer* timer) {
    
    // Get source from EEPROM and then save it to settings
    int source = hardware->eepromRead(EEPROM_ADDRESS_SOURCE);
    if (source < 0 || source > 3) {
        // TODO found unknown source, what to do?
        source = AUTOMATIC;
    }
    this->override_source = (enum source)source;

    // Get stability time from EEPROM and then save it to settings
    int stability_time = hardware->eepromRead(EEPROM_ADDRESS_STABILITY_TIME);
    if (stability_time < 0 || stability_time > 255) {
        stability_time = 30;
    }

    timer->set_timer(STABILITY_TIME, stability_time);

    // Get warmup time
    int warmup_time = hardware->eepromRead(EEPROM_ADDRESS_WARM_UP_TIME);
    if (warmup_time < 0 || warmup_time > 255) {
        warmup_time = 30;
    }

    timer->set_timer(WARM_UP_TIME, warmup_time);

    // Get transfer time
    int switching_delay_time = hardware->eepromRead(EEPROM_ADDRESS_SWITCHING_DELAY_TIME);
    if (switching_delay_time < 0 || switching_delay_time > 255) {
        switching_delay_time = 5;
    }

    timer->set_timer(SWITCHING_DELAY, switching_delay_time);
    

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

void Settings::add_big_num_buffer(int num) {

    // Get length of num
    int length = 0;
    int num_copy = num;

    int digits[NUM_BUFFER_MAX_LENGTH];

    while (num_copy > 0) {
        digits[length] = num_copy % 10;
        num_copy /= 10;
        length++;
    }

    // Now add the digits in reverse order to the buffer
    for (int i = length - 1; i >= 0; i--) {
        this->add_num_buffer(digits[i]);
    }
}

int Settings::get_num_buffer_int() {

    int* buffer = this->get_num_buffer()->buffer;
    int length = this->get_num_buffer()->length;

    int value = 0;
    for (int i = 0; i < length; i++) {
        value = value * 10 + buffer[i];
    }

    return value;

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
void Settings::commit_setting(Hardware *hardware, States state, uint8_t value)
{
    switch (state) {
        case SETTINGSMANUALDRIVE:
            hardware->eepromWrite(
                EEPROM_ADDRESS_SOURCE,
                value
            );
            break;
        case SETTINGSSTABILITYTIME:
            hardware->eepromWrite(
                EEPROM_ADDRESS_STABILITY_TIME,
                value
            );
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
