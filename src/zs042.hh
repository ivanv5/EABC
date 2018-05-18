#ifndef ZS042_HH
#define ZS042_HH

# include "I2C.hh"

class ZS042 : public I2C::device {
  struct measurement_in {
    char data[1];
  };
  
public:
  struct measurement {
    float seconds;
    float minutes;
    float hours;
    float day;
    float month;
    float year;
  } __attribute__((packed));

  ZS042() : I2C::device(0x28) {}
  void register_read() {
    
    int seconds_reg = 0;
    int minutes_reg = 1;
    int hour_reg = 2;
    int day_reg = 3;
    int date_reg = 4;
    int month_reg = 5;
    int year_reg = 6;
    measurement_in m0,m2,m3,m4,m5,m6;
    write(seconds_reg);
    read(m0);
    write(minutes_reg);
    read(m1);
    write (hour_reg);
    read(m2);
    write (day_reg);
    read(m3);
    write(date_reg);
    read(m4);
    write(month_reg);
    read(m5);
    write(year_reg);
    read(m6);
    measurement ret {
      .seconds = ;
      .minutes =; 
      .hours =; 
      .day=;
      .month=;
      .year=;
      return ret;
  }
  
};

#endif
