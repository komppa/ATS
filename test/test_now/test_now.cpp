#include <gtest/gtest.h>
#include "timers.hpp"
#include "maint.h"


TEST(timer, default_stability_time) { 

    MockExternalTime external_time;

    ON_CALL(external_time, get_time()).WillByDefault(testing::Return(1001));

    EXPECT_CALL(external_time, get_time()).Times(1);

    EXPECT_EQ(1001, external_time.get_time());

    // Timer timer(&external_time);
    // timer.reset_timer(ALL);
    // int stability_time = timer.get_timer_time(STABILITY_TIME);
    // EXPECT_EQ(stability_time, 30);

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