#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"
#include "FiniteStateMachine.hpp"
#include "states.h"


using namespace testing;


#ifdef UNIT_TEST
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
#endif



TEST(states, check_initial_state) {

    extern FSM sm;

    EXPECT_EQ(
        sm.getCurrentState().getStateName(),
        "UnknownStart"
    );

}
