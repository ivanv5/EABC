#ifndef BMP280_HH
#define BMP280_HH
# include "I2C.hh"
#include <iostream>

class bmp280 : public I2C::device {
public:
    enum slave_address : uint8_t {
	sdo_to_gnd = 0x76,
	sdo_to_vdd = 0x77
    };
    
    bmp280(slave_address addr = sdo_to_gnd) : I2C::device(addr) {
	read_register(0x88, _cal);
	char modo;
	read_register(0xf4, modo);
	std:: cout << "modo: " << (int)modo << std::endl;
	char mode[] = { 0xf4, 3 };
	write(mode);
    }
    
    float read_t() const {
	return (float)((read_t_fine() * 5 + 128) >> 8) / 100.f;
    }

    float read_p() const {
	int64_t t_fine = read_t_fine();
	int32_t adc_p = read_up();

	int64_t var1 = t_fine - 128000LL;
	int64_t var2 = var1 * var1 * (int64_t)_cal.p6;
	var2 += (var1 * (int64_t)_cal.p5) << 17;
	var2 += (int64_t)_cal.p4 << 35;
	var1 = ((var1 * var1 * (int64_t)_cal.p3) >> 8)
	    + ((var1 * (int64_t)_cal.p2) << 12);
	var1 = ((1LL << 47 + var1) * (int64_t) _cal.p1) >> 33;

	if (var1 == 0) return 0.f;

	int64_t p = 1048576LL - adc_p;
	p = ((p << 31) - var2) * 3125 / var1;
	var1 = ((int64_t)_cal.p9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t)_cal.p8 * p) >> 19;
	p = ((p + var1 + var2) >> 8) + ((int64_t)_cal.p7 << 4);
	return (float)p / 256.f;
    }
    
private:
    int32_t read_ut() const { return read_20b(0xfa); }

    int32_t read_up() const { return read_20b(0xf7); }

    int32_t read_20b(uint8_t reg) const {
	char data[3];
	read_register(reg, data);
	std::cout << "read20b " << std::hex
		  << (int)data[0] << " "
		  << (int)data[1] << " "
		  << (int)data[2] << std::endl;
	return data[0]<<12 | data[1] << 4 | data[2] >> 4;
    }
    
    int32_t read_t_fine() const {
	int adc_t = read_ut();
	int32_t var1 = ((adc_t >> 3) - ((int32_t)_cal.t1 << 1))
	    * ((int32_t)_cal.t2 >> 11);

	int32_t var2 = ((adc_t >> 4) - (int32_t)_cal.t1)
	    * (((adc_t >> 4) - (int32_t)_cal.t1) >> 12)
	    * ((int32_t)_cal.t3 >> 14);

	return var1 + var2;
    }
	    
    struct cal {
      uint16_t t1;
      int16_t t2, t3;
      uint16_t p1;
      int16_t p2, p3, p4, p5, p6, p7, p8, p9;
    } __attribute__((packed)) _cal;
};

#endif
