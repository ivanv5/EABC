#ifndef HYT221_HH
#define HYT221_HH

# include "I2C.hh"

class hyt221 : public I2C::device {
  struct measurement_internal {
    char data[4];
  };
  
public:
  struct measurement {
    float t;
    float h;
  } __attribute__((packed));

  hyt221() : I2C::device(0x28) {}
  measurement read() {
    measurement_internal m;
    read(m);

    measurement ret {
      .t = ((m.data[0] & 0x3f)<<8 | m.data[1]) * 100.f / 0x3fff;
      .h = (m.data[2]<<8 | m.data[3]&0xfc) * 165.f / 0xfffc - 40.f;
    return ret;
  }
  
};

#endif
