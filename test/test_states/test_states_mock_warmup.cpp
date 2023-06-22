#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, warmup_do_nothing_if_no_timeout) {

    extern State WarmUp;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(WarmUp, &deps);

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        WARMUP
    );

    sm.update();
    sm.update();


    MOCK_FAST_MILLIS(timer, 28 * 1000);

    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        WARMUP
    );

}

TEST(states, warmup_transfer_to_switchdelaytogen_if_timeout) {

    extern State WarmUp;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(WarmUp, &deps);

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        WARMUP
    );

    sm.update();
    sm.update();


    MOCK_FAST_MILLIS(timer, 31 * 1000);

    sm.update();
    sm.update();
    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getState(),
        SWITCHDELAYTOGEN
    );

}