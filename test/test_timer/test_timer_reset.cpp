#include <gtest/gtest.h>
#include "timers.hpp"


using namespace testing;


TEST(timer, reset_timer_all) { 
    Timer timer;
    timer.reset_timer(ALL);

    int stability_time = timer.get_remaining_time(STABILITY_TIME);
    int switching_delay = timer.get_remaining_time(SWITCHING_DELAY);
    int warm_up_time = timer.get_remaining_time(WARM_UP_TIME);

    EXPECT_EQ(stability_time, 30);
    EXPECT_EQ(switching_delay, 5);
    EXPECT_EQ(warm_up_time, 30);
}

TEST(timer, reset_timer_stability_time) { 
    Timer timer;
    timer.reset_timer(STABILITY_TIME);
    int stability_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);
}

TEST(timer, reset_timer_switching_delay) { 
    Timer timer;
    timer.reset_timer(SWITCHING_DELAY);
    int switching_delay = timer.get_remaining_time(SWITCHING_DELAY);
    EXPECT_EQ(switching_delay, 5);
}

TEST(timer, reset_timer_warm_up_time) { 
    Timer timer;
    timer.reset_timer(WARM_UP_TIME);
    int warm_up_time = timer.get_remaining_time(WARM_UP_TIME);
    EXPECT_EQ(warm_up_time, 30);
}
