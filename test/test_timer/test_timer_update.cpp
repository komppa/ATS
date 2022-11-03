#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "timers.hpp"
#include "external_time.h"
#include "maint.h"


using namespace testing;


TEST(timer, update_stability_time_when_active_and_remaining_over_zero) { 

    MockExternalTime external_time;
    Timer timer(&external_time);
    int stability_time;

    ON_CALL(external_time, get_time()).WillByDefault(Return(0));

    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);


    ON_CALL(external_time, get_time()).WillByDefault(Return(1001));

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 29);

}

TEST(timer, update_stability_time_when_inactive_and_remaining_over_zero) { 

    MockExternalTime external_time;
    Timer timer(&external_time);
    int stability_time;

    ON_CALL(external_time, get_time()).WillByDefault(Return(0));

    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);


    ON_CALL(external_time, get_time()).WillByDefault(Return(1001));

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 29);

}

TEST(timer, update_stability_time_when_active_and_remaining_zero) {

    MockExternalTime external_time;
    Timer timer(&external_time);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, true);

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    ON_CALL(external_time, get_time()).WillByDefault(Return(1001)); // "Wait" over a second

    timer.update();

    // Still, the min value should be 0
    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

}

TEST(timer, update_stability_time_when_inactive_and_remaining_zero) {

    MockExternalTime external_time;
    Timer timer(&external_time);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, false);  // Timer is not enabled

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    ON_CALL(external_time, get_time()).WillByDefault(Return(1001)); // "Wait" over a second

    timer.update();

    // Still, the min value should be 0
    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

}

TEST(timer, update_does_not_reset_flags_on_remaining_zero) {

    // TEST TODO there is no way to check whether timer is active * 
    GTEST_SKIP();

    MockExternalTime external_time;
    Timer timer(&external_time);
    int stability_time;

    timer.reset_timer(STABILITY_TIME);
    timer.set_timer(STABILITY_TIME, 0);
    timer.reset_timer(STABILITY_TIME);
    timer.enable_timer(STABILITY_TIME, false);  // Timer is not enabled

    timer.update();

    stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 0);

    ON_CALL(external_time, get_time()).WillByDefault(Return(1001)); // "Wait" over a second

    timer.update();

    // * Active flag for the timer should not be modified even if remaining value reaches zero
    // ...
}
