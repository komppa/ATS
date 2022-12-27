#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, switchdelaytogrid_transfer_to_normal_after_transfer_delay) {

    extern State SwitchDelayToGrid;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(SwitchDelayToGrid, &deps);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchDelayToGrid"
    );

    sm.update();

    MOCK_FAST_MILLIS(timer, 6 * 1000);

    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

}

TEST(states, switchdelaytogrid_do_not_transfer_to_normal_if_no_timeout) {

    extern State SwitchDelayToGrid;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(SwitchDelayToGrid, &deps);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchDelayToGrid"
    );

    sm.update();

    MOCK_FAST_MILLIS(timer, 4 * 1000);

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchDelayToGrid"
    );

}