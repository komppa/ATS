#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"
#include "settings.h"


using namespace testing;


TEST(states, transfer_to_waitgen_when_timeout) {

    extern State Stability;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Stability, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);
        
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

    sm.update();

    MOCK_FAST_MILLIS(timer, 31 * 1000);

    sm.update();
    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "WaitGen"
    );

}

TEST(states, do_not_transfer_to_waitgen_when_no_timeout) {

    extern State Stability;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Stability, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 0);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);
    
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

    sm.update();
    sm.update();

    // Wait under 30 seconds that is less than initial value of stability time
    MOCK_FAST_MILLIS(timer, 28 * 1000);  

    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

}

TEST(states, get_back_to_grid_if_it_returns_after_timeout) {

    extern State Stability;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Stability, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 240);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 0);
    
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

    sm.update();
    sm.update();

    // Wait under 30 seconds that is less than initial value of stability time
    MOCK_FAST_MILLIS(timer, 31 * 1000);  

    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Normal"
    );

}

TEST(states, switchdelaytogrid_when_both_sources_on) {

    extern State Stability;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Stability, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 240);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 240);
    
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

    sm.update();
    sm.update();

    // Wait under 30 seconds that is less than initial value of stability time
    MOCK_FAST_MILLIS(timer, 31 * 1000);  

    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "SwitchDelayToGrid"
    );

}

TEST(states, do_not_transfer_to_switchdelaytogrid_when_both_sources_on_but_no_timeout) {

    extern State Stability;

    MockHardware hardware;
    Timer timer(&hardware);

    Deps deps;
    deps = (Deps){
        .hardware = &hardware,
        .timer = &timer
    };

    FSM sm = FSM(Stability, &deps);

    MOCK_VOLTAGE(PIN_VOLTAGE_GRID, 240);
    MOCK_VOLTAGE(PIN_VOLTAGE_GENERATOR, 240);
    
    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

    sm.update();
    sm.update();

    // Wait under 30 seconds that is less than initial value of stability time
    MOCK_FAST_MILLIS(timer, 28 * 1000);

    sm.update();
    sm.update();
    

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "Stability"
    );

}