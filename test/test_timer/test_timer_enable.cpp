#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "external_time.h"
#include "maint.h"
#include "hardware.h"


using namespace testing;


TEST(timer, enable_counts_stability_down) { 

    MockHardware hardware;
    Timer timer(&hardware);
    int remaining_time;
    int initial_time;

    MOCK_MILLIS(0);

    timer.reset_timer(STABILITY_TIME);

    // Enable timer
    timer.enable_timer(STABILITY_TIME, true);
     
    timer.update();
    
    // Wait a second
    MOCK_MILLIS(1001);
    timer.update();

    remaining_time = timer.get_remaining_time(STABILITY_TIME);

    // Since timer has not been reseted, the remaining time should not
    // have the new value
    EXPECT_EQ(remaining_time, 29);

}

TEST(timer, disabled_do_not_count_down) { 

    MockHardware hardware;
    Timer timer(&hardware);
    int remaining_time;
    int initial_time;

    MOCK_MILLIS(0);

    timer.reset_timer(STABILITY_TIME);

    // Ensure that the timer is disabled
    timer.enable_timer(STABILITY_TIME, false);
     
    timer.update();
    
    // Wait a second
    MOCK_MILLIS(1001);
    timer.update();

    remaining_time = timer.get_remaining_time(STABILITY_TIME);

    // Since timer has not been reseted, the remaining time should not
    // have the new value
    EXPECT_EQ(remaining_time, 30);

}
