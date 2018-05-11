#ifndef BMP280_HH
#define BMP280_HH
# include "I2C.hh"
#include <thread>

class bmp280 : public I2C::device {
public:
    bmp280() : I2C::device(0x77) {
	read_register(0x88, _dig);
    }
    
    float read_t() const {
	return (float)((read_t_fine() * 5 + 128) >> 8) / 100.f;
    }

    float read_p() const {
	int64_t t_fine = read_t_fine();
	int32_t adc_p = read_up();

	int64_t var1 = t_fine - 128000LL;
	int64_t var2 = var1 * var1 * (int64_t)_dig.p6;
	var2 += (var1 * (int64_t)_dig.p5) << 17;
	var2 += (int64_t)_dig.p4 << 35;
	var1 = ((var1 * var1 * (int64_t)_dig.p3) >> 8)
	    + ((var1 * (int64_t)_dig.p2) << 12);
	var1 = ((1LL << 47 + var1) * (int64_t) _dig.p1) >> 33;

	if (var1 == 0) return 0.f;

	int64_t p = 1048576LL - adc_p;
	p = ((p << 31) - var2) * 3125 / var1;
	var1 = ((int64_t)_dig.p9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t)_dig.p8 * p) >> 19;
	p = ((p + var1 + var2) >> 8) + ((int64_t)_dig.p7 << 4);
	return (float)p / 256.f;
    }
    
private:
    int32_t read_ut() const { return read_20b(0xf7); }
    
    int32_t read_up() const { return read_20b(0xfa); }

    int32_t read_20b(uint8_t reg) const {
	char data[3];
	read_register(reg, data);
	return data[0]<<12 | data[1] << 4 | data[2] >> 4;
    }
    
    int32_t read_t_fine() const {
	int adc_t = read_ut();
	int32_t var1 = ((adc_t >> 3) - ((int32_t)_dig.t1 << 1))
	    * ((int32_t)_dig.t2 >> 11);

	int32_t var2 = ((adc_t >> 4) - (int32_t)_dig.t1)
	    * (((adc_t >> 4) - (int32_t)_dig.t1) >> 12)
	    * ((int32_t)_dig.t3 >> 14);

	return var1 + var2;
    }
	    
    struct dig {
      uint16_t t1;
      int16_t t2, t3;
      uint16_t p1;
      int16_t p2, p3, p4, p5, p6, p7, p8, p9;
    }__attribute__((packed));

    dig _dig;
};

#endif
