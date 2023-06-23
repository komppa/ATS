#include "writer.h"


#ifndef UNIT_TEST


int Writer::update() {

    if (
        (this->row_roll == false && this->second_row_roll == false) &&
        (this->variables.first.exists == false && this->variables.second.exists == false)
    ) {
        return 0;
    }


    if ((this->last_update + DISPLAY_UPDATE_INTERVAL) <= millis()) {

        this->lcd->clear();
    
        // Does the first row need rolling
        if (this->row.length() > DISPLAY_WIDTH) {

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
            String row_cut;
            // Scrolling not required for the first row.
            // Still, redraw everything for the first row since
            // everything is cleared on screen on every update.
            // Since dynamic variables can be injected to display,
            // this method is executed even if mode is RAW and scrolling
            // is not required. Because of that:
            // Check whether staticish text should start from left (RAW) or middle
            if (this->mode == MIDDLE) {
                this->lcd->setCursor(floor((DISPLAY_WIDTH - row.length()) / 2 ), 0);
            } else {
                this->lcd->setCursor(0, 0);
            }
            
            if (this->row[0] == '#') {
                this->lcd->write(byte(ICON_GRID));
                // Remove grid mark from string since icon has been writed
                // to the screen
                row_cut = this->row.substring(1);
            } else if (this->row[0] == '%') {
                this->lcd->write(byte(ICON_GENERATOR));
                row_cut = this->row.substring(1);
            } else if (this->row[0] == '&') {
                this->lcd->write(byte(ICON_LOAD));
                row_cut = this->row.substring(1);
            } else {
                row_cut = this->row;
            }

            this->lcd->print(row_cut);
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
            String secondrow_cut;
            // Scrolling not required for the second row.
            // Still, redraw everything for the second row since
            // everything is cleared on screen on every update.
            if (this->mode == MIDDLE) {
                this->lcd->setCursor(floor((DISPLAY_WIDTH - second_row.length()) / 2 ), 1);
            } else {
                this->lcd->setCursor(0, 1);
            }

            if (this->second_row[0] == '#') {
                this->lcd->write(byte(ICON_GRID));
                // Remove grid mark from string since icon has been writed
                // to the screen
                secondrow_cut = this->second_row.substring(1);
            } else if (this->second_row[0] == '%') {
                this->lcd->write(byte(ICON_GENERATOR));
                secondrow_cut = this->second_row.substring(1);
            } else if (this->second_row[0] == '&') {
                this->lcd->write(byte(ICON_LOAD));
                secondrow_cut = this->second_row.substring(1);
            } else {
                secondrow_cut = this->second_row;
            }
            
            this->lcd->print(secondrow_cut);
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

void Writer::write(String row) {
    this->write(row, "");
}

String Writer::variableToRow(writeRow row_select, int characters, String text, String variable) {

    // If lenght of injectable characters are over 3 (over default chars "{N}")
    // lenght must be extended from the end
    // See: line 153.

    // Add leading "zero" (empty chars) if input variable is shorter
    // than the reserved length for it
    String padded_variable = "";

    if ((int)(variable.length()) < characters) {
        for (int i = 0; i < (characters - (int)(variable.length())); i++) {
            padded_variable += " ";
        }
        padded_variable += variable;
    } else {
        padded_variable = variable;
    }

    if (row_select == FIRST) {
        return (String)
            text.substring(
                0,
                this->variables.first.start_index
            ) +
            padded_variable +   // Placeholder for variable that is not told
            text.substring(
                this->variables.first.end_index + characters,
                this->variables.first.initial_length + (characters - 3)  // Extend end of the string
            );
    }

    return (String)
        text.substring(
            0,
            this->variables.second.start_index
        ) +
        padded_variable +   // Placeholder for variable that is not told
        text.substring(
            this->variables.second.end_index + characters,
            this->variables.second.initial_length
    );
}

void Writer::write(String row, String second_row = "") {

    // Check whether there are variable placeholders
    this->variables.first.start_index = row.indexOf('{');

    if (this->variables.first.start_index == -1) {
        this->variables.first.exists = false;
        this->row = row;
    } else {
        this->variables.first.end_index = 
            this->variables.first.start_index;  // length value + end mark = 2 chars

        this->variables.first.length = (int)row.charAt(this->variables.first.start_index + 1) - '0';

        this->variables.first.initial_length = row.length();

        this->variables.first.exists = true;

        String v = "";
        for (int i = 0; i < this->variables.first.length; i++) {
            v += " ";
        }

        this->row = (String)(row.substring(
            0,
            this->variables.first.start_index
        ) +
        v +   // Placeholder for variable that is not told
        row.substring(
            this->variables.first.end_index + 3,
            row.length()
        ));
    }

    this->variables.second.start_index = second_row.indexOf('{');

    if (this->variables.second.start_index == -1) {
        this->variables.second.exists = false;
        this->second_row = second_row;
    } else {
        this->variables.second.end_index = 
            this->variables.second.start_index;  // length value + end mark = 2 chars

        this->variables.second.length = (int)second_row.charAt(this->variables.second.start_index + 1) - '0';

        this->variables.second.initial_length = second_row.length();

        this->variables.second.exists = true;

        String v = "";
        for (int i = 0; i < this->variables.second.length; i++) {
            v += " ";
        }

        this->second_row = (String)(second_row.substring(
            0,
            this->variables.second.start_index
        ) +
        v +   // Placeholder for variable that is not told
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
    
    String row_cut;
    String secondrow_cut;
    
    switch (this->mode) {

        // Just write start of the screen
        case RAW:
            this->lcd->setCursor(0,0);
            if (this->row[0] == '#') {
                this->lcd->write(byte(ICON_GRID));
                // Remove grid mark from string since icon has been writed
                // to the screen
                row_cut = this->row.substring(1);
            } else if (this->row[0] == '%') {
                this->lcd->write(byte(ICON_GENERATOR));
                row_cut = this->row.substring(1);
            } else if (this->row[0] == '&') {
                this->lcd->write(byte(ICON_LOAD));
                row_cut = this->row.substring(1);
            }
            this->lcd->print(row_cut.substring(0, DISPLAY_WIDTH));

            // Check whether there is the second row to be writed
            if (second_row.length() != 0) {
                this->lcd->setCursor(0,1);
                if (this->second_row[0] == '#') {
                    this->lcd->write(byte(ICON_GRID));
                    secondrow_cut = this->second_row.substring(1);
                } else if (this->second_row[0] == '%') {
                    this->lcd->write(byte(ICON_GENERATOR));
                    secondrow_cut = this->second_row.substring(1);
                } else if (this->second_row[0] == '&') {
                    this->lcd->write(byte(ICON_LOAD));
                    secondrow_cut = this->second_row.substring(1);
                }
                this->lcd->print(secondrow_cut.substring(0, DISPLAY_WIDTH));
            }

            break;
        
        // Text align center
        case MIDDLE:
            if (this->row.length() > DISPLAY_WIDTH) {
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

            // Check whether there is the second row to be writed
            if (this->second_row.length() != 0) {
                if (this->second_row.length() > DISPLAY_WIDTH) {
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
                this->lcd->print(this->second_row.substring(0, DISPLAY_WIDTH));
                // Save the second row staring index
                this->second_row_index = index_second + 4;
            }

            break;

        // On default, do nothing
        default:
            break;

    }
  
}


void Writer::variable(writeRow row_select, String var) {

    // TODO is there any reserved space for showing deynamic variable
    
    if (row_select == FIRST) {
        // TODO Check if the incoming var is longer than space
        // reserved for it
        this->row = variableToRow(row_select, this->variables.first.length, this->row, var);
    } else {
        this->second_row = variableToRow(row_select, this->variables.second.length, this->second_row, var);
    }
}


int Writer::set_write_to_row(writeRow new_write_row) {
    return 0;
}

#endif  // UNIT_TEST