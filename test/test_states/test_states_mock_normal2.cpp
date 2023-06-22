#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, transfer_to_normal_when_generator_goes_down) {

    extern State Normal2;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal2, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 250);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);
    
    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL2
    );

    sm.update();
    sm.update();
    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL
    );

}

TEST(states, do_not_transfer_anywhere_if_static) {

    extern State Normal2;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal2, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 250);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 250);

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL2
    );

    // At least enough of updates to ensure
    // keeping on normal state
    sm.update();
    sm.update();
    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL2
    );

}

TEST(states, normal2_to_waitgen_if_grid_goes_down) {

    extern State Normal2;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal2, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 250);

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        NORMAL2
    );

    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        WAITGEN
    );

}