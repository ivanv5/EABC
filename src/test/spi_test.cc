#include "SPI.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>

class SPITest: public testing::Test {
public:
  SPITest() {}
};

TEST_F(SPITest, TestExporter) {
    SPI::device adc(0);
}
