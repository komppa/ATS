#include <gtest/gtest.h>
#include "timers.hpp"
#include "maint.h"


using namespace testing;

/*
TEST(timer, default_stability_time) { 

    MockExternalTime external_time;

    ON_CALL(external_time, get_time()).WillByDefault(Return(1001));

    EXPECT_CALL(external_time, get_time()).Times(1);

    EXPECT_EQ(1001, external_time.get_time());

    // Timer timer(&external_time);
    // timer.reset_timer(ALL);
    // int stability_time = timer.get_timer_time(STABILITY_TIME);
    // EXPECT_EQ(stability_time, 30);

}
*/


TEST(timer, reset_timer_all) { 
    Timer timer;
    timer.reset_timer(ALL);

    int stability_time = timer.get_timer_time(STABILITY_TIME);
    int switching_delay = timer.get_timer_time(SWITCHING_DELAY);
    int warm_up_time = timer.get_timer_time(WARM_UP_TIME);

    EXPECT_EQ(stability_time, 30);
    EXPECT_EQ(switching_delay, 5);
    EXPECT_EQ(warm_up_time, 30);
}

TEST(timer, reset_timer_stability_time) { 
    Timer timer;
    timer.reset_timer(STABILITY_TIME);
    int stability_time = timer.get_timer_time(STABILITY_TIME);
    EXPECT_EQ(stability_time, 30);
}

TEST(timer, reset_timer_switching_delay) { 
    Timer timer;
    timer.reset_timer(SWITCHING_DELAY);
    int switching_delay = timer.get_timer_time(SWITCHING_DELAY);
    EXPECT_EQ(switching_delay, 5);
}

TEST(timer, reset_timer_warm_up_time) { 
    Timer timer;
    timer.reset_timer(WARM_UP_TIME);
    int warm_up_time = timer.get_timer_time(WARM_UP_TIME);
    EXPECT_EQ(warm_up_time, 30);
}


#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock();
}

void loop()
{
  // Run tests
  if (RUN_ALL_TESTS())
  ;

  // sleep for 1 sec
  delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
    ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif