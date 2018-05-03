#include "ads1118.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;


class ADS1118Test: public testing::Test {
public:
  ADS1118Test() {}
};

TEST_F(ADS1118Test, TestExporter) {
    ads1118 adc(0); //declara adc como objeto de la clase ads1118
    ads1118 s(0);
    s= adc.in(0);
    
    std::cout << adc.in(0) << std::endl;//preguntar a Paco
}
