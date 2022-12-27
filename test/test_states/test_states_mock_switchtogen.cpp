#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;
using ::testing::AtLeast;


TEST(states, switchtogen_ensure_contactor_calls) {

    extern State SwitchToGen;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    EXPECT_CALL(hardware, digitalWrite(PIN_CONTACTOR_GRID, false)).Times(1);
    EXPECT_CALL(hardware, digitalWrite(PIN_CONTACTOR_GENERATOR, true)).Times(1);

    FSM sm = FSM(SwitchToGen, &deps);

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchToGen"
    );

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchToGen"
    );

}