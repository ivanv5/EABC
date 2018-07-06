#ifndef ZS042_HH
#define ZS042_HH

#include "I2C.hh"

class ZS042 : public I2C::device {  
public:
	struct measurement {
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t day;
		uint8_t date;
		uint8_t month;
		uint8_t year;
	} __attribute__((packed));

	ZS042() : I2C::device(0x68) {}

	measurement  clock_read() {
		measurement m;
		read_register(0, m);
		for (auto& byte : reinterpret_cast<uint8_t(&)[sizeof m]>(m))
			byte = from_bcd(byte);
  		return m;
	}

private: 
	static uint8_t from_bcd(uint8_t s){
		return (s>>4)*10 + (s&0xf);
	}
 
};

#endif
