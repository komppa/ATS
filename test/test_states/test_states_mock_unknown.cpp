#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, keep_normal_on_when_grid_on) {

    extern State UnknownStart;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(UnknownStart, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 250);
        
    EXPECT_EQ(
        sm.getCurrentState().getState(),
        UNKNOWNSTART
    );

    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL
    );

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL
    );

}
