#include "I2C.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>

class I2CTest: public testing::Test {
public:
  I2CTest() {}
};

TEST_F(I2CTest, TestExporter) {
    I2C::device adc(0);
}
