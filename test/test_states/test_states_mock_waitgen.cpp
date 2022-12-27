#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, waitgen_transfer_out_when_gen_starts) {

    extern State WaitGen;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(WaitGen, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 240);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WaitGen"
    );

    sm.update();
    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WarmUp"
    );

}

TEST(states, waitgen_transfer_back_to_stability_if_grid_restores) {

    extern State WaitGen;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(WaitGen, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 240);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WaitGen"
    );

    sm.update();
    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

}

TEST(states, waitgen_wait_if_all_down) {

    extern State WaitGen;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(WaitGen, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WaitGen"
    );

    sm.update();
    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WaitGen"
    );

}