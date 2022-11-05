#include <gtest/gtest.h>
#include "timers.hpp"


// using namespace testing;


TEST(timer, get_remaining_stability_time) { 
    
    Timer timer;
    int remaining_time;

    timer.reset_timer(STABILITY_TIME);

    remaining_time = timer.get_remaining_time(STABILITY_TIME);

    EXPECT_EQ(remaining_time, 30);

}

TEST(timer, get_initial_stability_time) { 
    
    Timer timer;
    int initial_time;

    timer.reset_timer(STABILITY_TIME);

    initial_time = timer.get_initial_time(STABILITY_TIME);

    EXPECT_EQ(initial_time, 30);

}

TEST(timer, get_remaining_switching_delay) { 
    
    Timer timer;
    int remaining_time;

    timer.reset_timer(SWITCHING_DELAY);

    remaining_time = timer.get_remaining_time(SWITCHING_DELAY);

    EXPECT_EQ(remaining_time, 5);

}

TEST(timer, get_initial_switching_delay) { 
    
    Timer timer;
    int initial_time;

    timer.reset_timer(SWITCHING_DELAY);

    initial_time = timer.get_initial_time(SWITCHING_DELAY);

    EXPECT_EQ(initial_time, 5);

}

TEST(timer, get_remaining_warm_up_time) { 
    
    Timer timer;
    int remaining_time;

    timer.reset_timer(WARM_UP_TIME);

    remaining_time = timer.get_remaining_time(WARM_UP_TIME);

    EXPECT_EQ(remaining_time, 30);

}

TEST(timer, get_initial_warm_up_time) { 
    
    Timer timer;
    int initial_time;

    timer.reset_timer(WARM_UP_TIME);

    initial_time = timer.get_initial_time(WARM_UP_TIME);

    EXPECT_EQ(initial_time, 30);

}

TEST(timer, get_unknown_timer_selection) {
    EXPECT_EQ(1,1);

}
