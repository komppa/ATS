#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


extern State UnknownStart;
extern State Normal;
extern State Stability;
extern State WaitGen;
extern State WarmUp;
extern State SwitchDelayToGen;
extern State SwitchToGen;
extern State DetachGen;
extern State Normal2;
extern State SwitchDelayToGrid;



TEST(states, normal_after_unknown) {

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
        
    ON_CALL(hardware, getVoltageAC(PIN_VOLTAGE_GRID)).WillByDefault(Return(250));
    
        
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "UnknownStart"
    );

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "UnknownStart"
    );

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

    // extern FSM sm;
    // MockHardware hardware;
    // Timer timer(&hardware);

    // // Reset all timers
    // timer.reset_timer(ALL);

    // sm.update();

    // EXPECT_EQ(
    //     sm.getCurrentState().getStateName(),
    //     "UnknownStart"
    // );

    // sm.update();

    // EXPECT_EQ(
    //     sm.getCurrentState().getStateName(),
    //     "Normal"
    // );

    EXPECT_EQ(
        1+1,
        2
    );

}

TEST(states, normal_after_unknown2) {

    // MockHardware hardware;

    // extern FSM sm;
    // sm.transitionTo(Normal);

    // sm.update();

    // EXPECT_EQ(
    //     sm.getCurrentState().getStateName(),
    //     "Normal"
    // );

    // ON_CALL(hardware, getVoltageAC(PIN_VOLTAGE_GRID)).WillByDefault(Return(240));
    // ON_CALL(hardware, getVoltageAC(PIN_VOLTAGE_GENERATOR)).WillByDefault(Return(240));

    // sm.update();

    // EXPECT_EQ(
    //     sm.getCurrentState().getStateName(),
    //     "Normal2"
    // );

    /*
    MockHardware hardware;

    FSM sm = FSM(Normal);

    sm.update();

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );
    */

    // ON_CALL(hardware, getVoltageAC(PIN_VOLTAGE_GRID)).WillByDefault(Return(240));
    // ON_CALL(hardware, getVoltageAC(PIN_VOLTAGE_GENERATOR)).WillByDefault(Return(240));

    // sm.update();

    // EXPECT_EQ(
    //     sm.getCurrentState().getStateName(),
    //     "Normal2"
    // );

}