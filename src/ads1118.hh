#ifndef ADS1118_HH
#define ADS1118_HH

# include "SPI.hh"

class ads1118 {

    // CFG register is stored MSB first
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
    
    ads1118(uint8_t addr) : _spi(addr) {//DEFINICION CONTRUCTOR ADS1118
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

    int16_t in(uint8_t channel) const { //IN LEER.
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
	return _xfer(cfg);
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
	return _xfer(cfg);
    }
    
private:
    int16_t _xfer(const config_register& cfg) const {
	unsigned char data[2];
	_spi.xfer(cfg, data);
	return (int16_t) (data[0] << 8 | data[1]);
    }

    SPI::device _spi;
};

#endif
