#ifndef ADS1118_HH
#define ADS1118_HH

# include "SPI.hh"

class ads1118 {


    struct config_register {
	unsigned mode:     1;
	unsigned pga:      3;
	unsigned mux:      3;
	unsigned ss:       1;

	unsigned reserved: 1;
	unsigned nop:      2;
	unsigned pull_up:  1;
	unsigned ts_mode:  1;
	unsigned rate:     3;
    } __attribute__((packed));
    
public:
    ads1118(uint8_t addr) : _spi(addr) {
	config_register cfg = {
	  .mode = 1,
	  .pga = 0,
	  .mux = 0,
	  .ss = 0,
	  .reserved = 1,
	  .nop = 1,
	  .pull_up = 1,
	  .ts_mode = 0,
	  .rate = 3,
	};
	_spi.write(cfg);
    }

    int16_t in(uint8_t channel) const {
	config_register cfg = {
	  .mode = 1,
	  .pga = 0,
	  .mux = 4u + channel,
	  .ss = 1,
	  .reserved = 1,
	  .nop = 1,
	  .pull_up = 1,
	  .ts_mode = 0,
	  .rate = 3,
	};
	int16_t data;
	_spi.xfer(cfg, data);
	return data;
    }

    int16_t t() const {
	config_register cfg = {
	  .mode = 1,
	  .pga = 0,
	  .mux = 0,
	  .ss = 0,
	  .reserved = 1,
	  .nop = 1,
	  .pull_up = 1,
	  .ts_mode = 1,
	  .rate = 3,
	};
	int16_t data;
	_spi.xfer(cfg, data);
	return data;
    }
    
private:
    SPI::device _spi;
};

#endif
