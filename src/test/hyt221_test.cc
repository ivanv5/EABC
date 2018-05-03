#include "HYT221.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>



class HYT221Test: public testing::Test {
public:
  HYT221Test() {}
};

TEST_F(HYT221Test, TestExporter) {
  hyt221 LS;
};
