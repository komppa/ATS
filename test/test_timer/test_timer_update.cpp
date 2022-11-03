#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "external_time.h"
#include "maint.h"
#include "hardware.h"


using namespace testing;


TEST(timer, update_stability_time_when_active_and_remaining_over_zero) { 

    MockHardware hardware;
    Timer timer(&hardware);
    int stability_time;

    // ON_CALL(hardware, millis()).WillByDefault(Return(0));
    // ON_CALL(external_time, get_time()).WillByDefault(Return(0));
    // MOCK_MILLIS(external_time, 0);
    MOCK_MILLIS(0);

    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);


    // ON_CALL(hardware, millis()).WillByDefault(Return(1001));
    // ON_CALL(external_time, get_time()).WillByDefault(Return(1001));
    // MOCK_MILLIS(external_time, 1001);
    MOCK_MILLIS(1001);
    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 29);

}

TEST(timer, update_stability_time_when_inactive_and_remaining_over_zero) { 

    MockHardware hardware;
    Timer timer(&hardware);
    int stability_time;

    MOCK_MILLIS(0);

    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);


    MOCK_MILLIS(1001);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 29);

}

TEST(timer, update_stability_time_when_active_and_remaining_zero) {

    MockHardware hardware;
    Timer timer(&hardware);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    MOCK_MILLIS(1001);  // "Wait" over a second

    timer.update();

    // Still, the min value should be 0
    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

}

TEST(timer, update_stability_time_when_inactive_and_remaining_zero) {
    
    MockHardware hardware;
    Timer timer(&hardware);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, false);  // Timer is not enabled

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    MOCK_MILLIS(1001);  // "Wait" over a second

    timer.update();

    // Still, the min value should be 0
    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

}

TEST(timer, update_does_not_reset_flags_on_remaining_zero) {

    // TEST TODO there is no way to check whether timer is active * 
    GTEST_SKIP();

    MockHardware hardware;
    Timer timer(&hardware);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, false);  // Timer is not enabled

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    MOCK_MILLIS(1001);

    timer.update();

    // * Active flag for the timer should not be modified even if remaining value reaches zero
    // ...
}
