#ifndef ADS1118_HH
#define ADS1118_HH

# include "SPI.hh"

class ads1118  {

    struct config_register __attribute__((packed)) {
	unsigned ss:       1;
	unsigned mux:      3;
	unsigned pga:      3;
	unsigned mode:     1;
	unsigned rate:     3;
	unsigned ts_mode:  1;
	unsigned pull_up:  1;
	unsigned nop:      2;
	unsigned reserved: 1;
    };
    
public:
    ads1118(uint8_t addr) : _spi(addr) {
	config_register cfg = { .ss = 0,
				.mux = 0,
				.pga = 0,
				.mode = 1,
				.rate = 3,
				.ts_mode = 1,
				.pull_up = 1,
				.nop = 1,
				.reserved = 1};
	_spi.write(cfg);
    }

    int16_t in(uint8_t channel) const {
	config_register cfg = { .ss = 1,
				.mux = 4 + channel,
				.pga = 0,
				.mode = 1,
				.rate = 3,
				.ts_mode = 1,
				.pull_up = 1,
				.nop = 1,
				.reserved = 1};
	_spi.xfer(cfg, data);
	return data;
    }

    double t() const {
	config_register cfg = { .ss = 0, .mux = 0, .pga = 0, .mode = 1, .rate = 3, .ts_mode = 1, .pull_up = 1, .nop = 1, .reserved = 1};
	int16_t data;
	
	_spi.xfer(cfg, data);
	return data;
    }
    
private:
    SPI::device _spi;
};

#endif
