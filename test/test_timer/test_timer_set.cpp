#include <gtest/gtest.h>
#include "timers.hpp"


using namespace testing;

#define RANDOM_NUM 7252


TEST(timer, set_stability_time) {

    Timer timer;
    int remaining_time;
    int initial_time;

    timer.reset_timer(STABILITY_TIME);

    timer.set_timer(STABILITY_TIME, RANDOM_NUM);

    remaining_time = timer.get_remaining_time(STABILITY_TIME);
    
    // Do not match remaining time to be new time since timer
    // has not been reset to new initial time yet 
    EXPECT_NE(remaining_time, RANDOM_NUM);

    // And now match
    timer.reset_timer(STABILITY_TIME);
    remaining_time = timer.get_remaining_time(STABILITY_TIME);
    EXPECT_EQ(remaining_time, RANDOM_NUM);

    // Has initial time changed as it should
    initial_time = timer.get_initial_time(STABILITY_TIME);
    EXPECT_EQ(initial_time, RANDOM_NUM);

}

TEST(timer, set_switching_delay) { 

    Timer timer;
    int remaining_time;
    int initial_time;

    timer.reset_timer(SWITCHING_DELAY);

    timer.set_timer(SWITCHING_DELAY, RANDOM_NUM);

    remaining_time = timer.get_remaining_time(SWITCHING_DELAY);
    
    // Do not match remaining time to be new time since timer
    // has not been reset to new initial time yet 
    EXPECT_NE(remaining_time, RANDOM_NUM);

    // And now match
    timer.reset_timer(SWITCHING_DELAY);
    remaining_time = timer.get_remaining_time(SWITCHING_DELAY);
    EXPECT_EQ(remaining_time, RANDOM_NUM);

    // Has initial time changed as it should
    initial_time = timer.get_initial_time(SWITCHING_DELAY);
    EXPECT_EQ(initial_time, RANDOM_NUM);

}

TEST(timer, set_warm_up_time) {

    Timer timer;
    int remaining_time;
    int initial_time;

    timer.reset_timer(WARM_UP_TIME);

    timer.set_timer(WARM_UP_TIME, RANDOM_NUM);

    remaining_time = timer.get_remaining_time(WARM_UP_TIME);
    
    // Do not match remaining time to be new time since timer
    // has not been reset to new initial time yet 
    EXPECT_NE(remaining_time, RANDOM_NUM);

    // And now match
    timer.reset_timer(WARM_UP_TIME);
    remaining_time = timer.get_remaining_time(WARM_UP_TIME);
    EXPECT_EQ(remaining_time, RANDOM_NUM);

    // Has initial time changed as it should
    initial_time = timer.get_initial_time(WARM_UP_TIME);
    EXPECT_EQ(initial_time, RANDOM_NUM);

}

TEST(timer, set_all_timer_selection) {

    // TEST TODO since timer 'ALL' is not implemented for every method
    GTEST_SKIP();

    Timer timer;
    int remaining_time;
    int initial_time;

    timer.reset_timer(ALL);

    timer.set_timer(ALL, RANDOM_NUM);

    remaining_time = timer.get_remaining_time(ALL);
    
    // Do not match remaining time to be new time since timer
    // has not been reset to new initial time yet 
    EXPECT_NE(remaining_time, RANDOM_NUM);

    // And now match
    timer.reset_timer(ALL);
    remaining_time = timer.get_remaining_time(ALL);
    EXPECT_EQ(remaining_time, RANDOM_NUM);

    // Has initial time changed as it should
    initial_time = timer.get_initial_time(ALL);
    EXPECT_EQ(initial_time, RANDOM_NUM);

}
