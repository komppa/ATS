#include "writer.h"


#ifndef UNIT_TEST


int Writer::update() {

    if (this->row_roll == false && this->second_row_roll == false) {
        return 0;
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
                this->lcd->print(this->row.substring(
                    0,
                    (-1) * (this->row_index - DISPLAY_WIDTH)
                ));
            } else {
                // Row should be negative value but since display does not support
                // negative values, it must be substringed on the fly
                this->lcd->setCursor(0, 0);
                this->lcd->print(this->row.substring(
                    (-1) * this->row_index,
                    (-1) * this->row_index + DISPLAY_WIDTH
                ));
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
                this->lcd->print(this->second_row.substring(
                    0, 
                    (-1) * (this->second_row_index - DISPLAY_WIDTH)
                ));
            } else {
                // Row should be negative value but since display does not support
                // negative values, it must be substringed on the fly
                this->lcd->setCursor(0, 1);
                this->lcd->print(this->second_row.substring(
                    (-1) * this->second_row_index,
                    (-1) * this->second_row_index + DISPLAY_WIDTH
                ));
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

String variableToRow(Writer *w, writeRow row_select, String text, String variable) {
    
    if (row_select == FIRST) {
        return (String)
            text.substring(
                0,
                w->variables.first.start_index
            ) +
            variable +   // Placeholder for variable that is not told
            text.substring(
                w->variables.first.end_index + 1,
                w->variables.first.initial_length
        );
    }

    return (String)
        text.substring(
            0,
            w->variables.second.start_index
        ) +
        variable +   // Placeholder for variable that is not told
        text.substring(
            w->variables.second.end_index + 1,
            w->variables.second.initial_length
    );
}

int Writer::write(String row, String second_row = "") {

    // Check whether there are variable placeholders
    this->variables.first.start_index = row.indexOf('{');

    if (this->variables.first.start_index == -1) {
        this->variables.first.exists = false;
    } else {
        this->variables.first.end_index = 
            this->variables.first.start_index;  // length value + end mark = 2 chars

        this->variables.first.length = (int)row.charAt(this->variables.first.start_index + 1) - '0';

        this->variables.first.initial_length = row.length() - 2;

        this->variables.first.exists = true;

        this->row = (String)(row.substring(
            0,
            this->variables.first.start_index
        ) +
        " " +   // Placeholder for variable that is not told
        row.substring(
            this->variables.first.end_index + 3,
            row.length()
        ));
    }

    this->variables.second.start_index = second_row.indexOf('{');

    if (this->variables.second.start_index == -1) {
        this->variables.second.exists = false;
    } else {
        this->variables.second.end_index = 
            this->variables.second.start_index;  // length value + end mark = 2 chars

        this->variables.second.length = (int)second_row.charAt(this->variables.second.start_index + 1) - '0';

        this->variables.second.initial_length = second_row.length() - 2;

        this->variables.second.exists = true;

        this->second_row = (String)(second_row.substring(
            0,
            this->variables.second.start_index
        ) +
        " " +   // Placeholder for variable that is not told
        second_row.substring(
            this->variables.second.end_index + 3,
            second_row.length()
        ));
    }


    // Mark if text in some row is longer than the display is
    if (this->row.length() > DISPLAY_WIDTH) {
        this->row_roll = true;
    } else {
        this->row_roll = false;
    }
    
    if (this->second_row.length() > DISPLAY_WIDTH) {
        this->second_row_roll = true;
    } else {
        this->second_row_roll = false;
    }


    this->lcd->clear();
    
    // Index stores which index is used to start
    // writing text on screen for each row.
    // If text is longer than the screen of the device,
    // it is not reasonable to align text center of the screen
    // that is why index is then set to be zero.
    // First row
    signed int index = floor((DISPLAY_WIDTH - row.length()) / 2 );
    if (index > DISPLAY_WIDTH) index = DISPLAY_WIDTH;

    if (row.length() > DISPLAY_WIDTH) index = 0;

    // Second row
    signed int index_second = floor((DISPLAY_WIDTH - second_row.length()) / 2 );
    if (index_second > DISPLAY_WIDTH) index_second = DISPLAY_WIDTH;

    if (second_row.length() > DISPLAY_WIDTH) index_second = 0;
    
    
    switch (this->mode) {

        // Just write start of the screen
        case RAW:
            this->lcd->setCursor(0,0);
            this->lcd->print(this->row.substring(0, DISPLAY_WIDTH));

            // Check whether there is the second row to be writed
            if (second_row.length() != 0) {
                this->lcd->setCursor(0,1);
                this->lcd->print(second_row);
            }

            break;
        
        // Text align center
        case MIDDLE:
            if (row.length() > DISPLAY_WIDTH) {
                this->lcd->setCursor(
                    index + 4,
                    0
                );
            } else {
                this->lcd->setCursor(
                    index,
                    0
                );
            }
            this->lcd->print(this->row.substring(0, DISPLAY_WIDTH));
            // Save the starting postion for the first row text
            this->row_index = index + 4;
            if (this->variables.first.exists == false) {
                this->row = row;
            }

            // Check whether there is the second row to be writed
            if (second_row.length() != 0) {
                if (second_row.length() > DISPLAY_WIDTH) {
                    this->lcd->setCursor(
                        index_second + 4,
                        1
                    );
                } else {
                    this->lcd->setCursor(
                        index_second,
                        1
                    );
                }
                this->lcd->print(second_row.substring(0, DISPLAY_WIDTH));
                // Save the second row text if not rolling
                if (this->variables.second.exists == false) {
                    this->second_row = second_row;
                }
                // Save the second row staring index
                this->second_row_index = index_second + 4;
            }

            break;

        // On default, do nothing
        default:
            break;

        return 0;

    }
  
}


int Writer::variable(writeRow row_select, String var) {


    if (row_select == FIRST) {

        // Check if the incoming var is longer than space
        // reserved for it
        if (var.length() > this->variables.first.length) {
            return -1;
        }

        this->row = variableToRow(this, row_select, this->row, var);

    } else {

        if (var.length() > this->variables.second.length) {
            return -1;
        }

        this->second_row = variableToRow(this, row_select, this->second_row, var);
        
    }
}


int Writer::set_write_to_row(writeRow new_write_row) {
    return 0;
}

#endif  // UNIT_TEST