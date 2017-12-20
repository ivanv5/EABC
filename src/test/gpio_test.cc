#include "GPIO.hh"
#include "gmock/gmock.h"

class GPIOTest: public testing::Test {
public:
  GPIOTest() : gpio18(18) {}
  GPIO::gpio gpio18;
};

TEST_F(GPIOTest, TestExporter) {
    gpio18.set(true);
    ASSERT_EQ(gpio18.get(), true);
    gpio18.set(false);
    ASSERT_EQ(gpio18.get(), false);
}
