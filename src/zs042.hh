#ifndef ZS042_HH
#define ZS042_HH
#include "I2C.hh"
#include <iostream>
#include <fstream>
#include <unistd.h>

class ZS042 : public I2C::device {
  
public:
   struct measurement {
   float seconds;
   float minutes;
   float hours;
   float date;
   float month;
   float year;
   } __attribute__((packed));

  ZS042() : I2C::device(0x68) {}
  measurement  clock_read() {
   
    read_register(seconds_reg,m0);
    read_register(minutes_reg,m1);
    read_register(hour_reg,m2);
    read_register(date_reg,m3);
    read_register(month_reg,m4);
    read_register(year_reg,m5);
    float seconds_in = internal_read(m0);
    float minutes_in = internal_read(m1);
    float hours_in = internal_read(m2);
    float date_in = internal_read(m3);
    float month_in = internal_read(m4);
    float year_in = internal_read(m5)+2000;
  
    measurement ret{
      seconds_in,
      minutes_in,
      hours_in,
      date_in,
      month_in,
      year_in
	
    };
    return ret;
       }
private:

    uint8_t m0,m1,m2,m3,m4,m5;    
    char seconds_reg = 0x00;
    char minutes_reg = 0x01;
    char hour_reg = 0x02;
    char date_reg = 0x04;
    char month_reg = 0x05;
    char year_reg = 0x06;
 
  float internal_read(uint8_t s){
    uint8_t msb_s= (s>>4);
    uint8_t lsb_s=(s<<4);
    lsb_s= lsb_s>>4;
    float msb_sfl=msb_s*10;
    float total= msb_sfl+lsb_s;
    return total;
  }
 
};

#endif
