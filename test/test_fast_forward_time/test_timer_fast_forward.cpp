#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "hardware.h"


using namespace testing;


TEST(timer, fast_forward_time) { 

    MockHardware hardware;
    Timer timer(&hardware);
    int remaining_time;
    int initial_time;

    MOCK_MILLIS(0);

    timer.reset_timer(STABILITY_TIME);

    // Enable timer
    timer.enable_timer(STABILITY_TIME, true);

    remaining_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(remaining_time, 30);

    
    // Wait a 3 seconds
    MOCK_FAST_MILLIS(timer, 3 * 1000);

    remaining_time = timer.get_remaining_time(STABILITY_TIME);

    // Since timer has not been reseted, the remaining time should not
    // have the new value
    EXPECT_EQ(remaining_time, 30-3);

}
