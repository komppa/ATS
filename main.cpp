#include "FiniteStateMachine.hpp"
#include "states.h"
#include <stdio.h>
#include <unistd.h>


extern FSM sm;
extern Timer timer;


int main() {


    timer.reset_timer(ALL);

    while(1) {
        timer.update(); // Timer
        sm.update();    // State machine
    }

}