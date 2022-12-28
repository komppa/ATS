#include "writer.h"


#ifndef UNIT_TEST


int Writer::update() {

    if (this->row_roll == false && this->second_row_roll == false) {
        return;
    }


    if ((this->last_update + DISPLAY_UPDATE_INTERVAL) <= millis()) {

        this->lcd->clear();
    
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
                this->lcd->setCursor(this->row_index, 0);
                // Substringin that the string wont appear to the second row
                this->lcd->print(this->row.substring(0, (-1) * (this->row_index - DISPLAY_WIDTH)));
            } else {
                // Row should be negative value but since display does not support
                // negative values, it must be substringed on the fly
                this->lcd->setCursor(0, 0);
                this->lcd->print(this->row.substring((-1) * this->row_index));
            }

        } else {
            // Scrolling not required for the first row.
            // Still, redraw everything for the first row since
            // everything is cleared on screen on every update.
            this->lcd->setCursor(floor((DISPLAY_WIDTH - row.length()) / 2 ), 0);
            this->lcd->print(this->row);
        }

        // Does the second row need rolling
        if (this->second_row.length() > DISPLAY_WIDTH) {

            if (this->second_row_index == (signed int)((-1) * this->second_row.length())) {
                this->second_row_index = DISPLAY_WIDTH - 1;
            }

            this->second_row_index--;

            if (this->second_row_index > 0) {
                this->lcd->setCursor(this->second_row_index, 1);
                // Substringin that the string wont appear to the second row
                //lcd.print(this->second_row.substring(0, (-1) * (this->second_row.length() - DISPLAY_WIDTH)));
                this->lcd->print(this->second_row.substring(0, (-1) * (this->second_row_index - DISPLAY_WIDTH)));
            } else {
                // Row should be negative value but since display does not support
                // negative values, it must be substringed on the fly
                this->lcd->setCursor(0, 1);
                this->lcd->print(this->second_row.substring((-1) * this->second_row_index));
            }
        

        } else {
            // Scrolling not required for the second row.
            // Still, redraw everything for the second row since
            // everything is cleared on screen on every update.
            this->lcd->setCursor(floor((DISPLAY_WIDTH - second_row.length()) / 2 ), 1);
            this->lcd->print(this->second_row);
        }

        this->last_update = millis();

    }

    return 0;

}

Writer::Writer(LiquidCrystal *lcd) {
    this->lcd = lcd;
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

int Writer::setMode(writeMode new_mode) {
    this->mode = new_mode;
    return 0;
}

int Writer::clear() {
    this->lcd->clear();
    return 0;
}

int Writer::write(String row) {
    this->write(row, "");
}


int Writer::write(String row, String second_row = "") {

    this->lcd->clear();
    
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
        this->lcd->setCursor(0,0);
        this->lcd->print(row);

        // Check whether there is the second row to be writed
        if (second_row.length() != 0) {
            this->lcd->setCursor(0,1);
            this->lcd->print(second_row);
            this->row = row;
        }

        break;
        
        // Text align center
        case MIDDLE:
        this->lcd->setCursor(
            index,
            0
        );
        this->lcd->print(row);
        // Save writed text
        this->row = row;
        // Save the starting postion for the first row text
        this->row_index = index;

        // Check whether there is the second row to be writed
        if (second_row.length() != 0) {
            this->lcd->setCursor(
            index_second,
            1
            );
            this->lcd->print(second_row);
            // Save the second row text
            this->second_row = second_row;
            // Save the second row staring index
            this->second_row_index = index_second;
        }

        break;

        // On default, do nothing
        default:
        break;

        return 0;

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


int Writer::set_write_to_row(writeRow new_write_row) {
    return 0;
}

#endif  // UNIT_TEST