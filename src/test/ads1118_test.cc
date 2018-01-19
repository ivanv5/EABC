#include "ads1118.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>
#include <iostream>

class ADS1118Test: public testing::Test {
public:
  ADS1118Test() {}
};

TEST_F(ADS1118Test, TestExporter) {
    bcm2835_init();

    ads1118 adc(0);
    std::cout << adc.in(0) << std::endl;

    bcm2835_close();
}
