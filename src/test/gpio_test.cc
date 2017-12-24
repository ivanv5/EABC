#include "GPIO.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>

class GPIOTest: public testing::Test {
public:
  GPIOTest() {}
};

TEST_F(GPIOTest, TestExporter) {
    GPIO::gpio gpio18(18);
    gpio18.set(true);
    ASSERT_EQ(gpio18.get(), true);
    gpio18.set(false);
    ASSERT_EQ(gpio18.get(), false);
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
