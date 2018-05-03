#include "HYT221.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>



class zs042Test: public testing::Test {
public:
  zs042Test() {}
};

TEST_F(zs042Test, TestExporter) {
  zs042 LS(0);
  zs042::register read();
   
    };
};
