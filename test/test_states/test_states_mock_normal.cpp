#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, transfer_to_normal2_when_both_sources_up) {

    extern State Normal;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 250);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 250);
    
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal2"
    );

}

TEST(states, do_not_transfer_anywhere_if_grid_up) {

    extern State Normal;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 250);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

    // At least enough of updates to ensure
    // keeping on normal state
    sm.update();
    sm.update();
    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

}

TEST(states, transfer_to_stability_when_grid_goes_down) {

    extern State Normal;

    MockHardware hardware;
    Timer timer(&hardware);

    timer.reset_timer(ALL);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Normal, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

}