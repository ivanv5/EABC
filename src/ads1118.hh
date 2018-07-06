#ifndef ADS1118_HH
#define ADS1118_HH

#include "SPI.hh"
#include <thread>

template <class pin_ctrl = RPi::spi_ctrl>
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
    
	/*! \brief Construct a new ads1118
	*/
    ads1118(uint8_t addr) : _spi(addr) {
		config_register cfg = {
			.mode = 1,
			.pga = 1,
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

	/*! \brief Reads last conversion from a given channel.
	*/
	int16_t in(uint8_t channel) const {
		using namespace std::chrono_literals;
		config_register cfg = {
			.mode = 1,
			.pga = 1,
			.mux = channel,
			.ss = 0,
			.reserved = 1,
			.nop = 1,
			.pull_up = 1,
			.ts_mode = 0,
			.rate = 3,
		};
		_spi.write(cfg);
		std::this_thread::sleep_for(125ms);
		return _xfer(cfg);
	}

	double in_V(uint8_t channel) const {
		return in(channel) * 4.096 / 32768.;
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


	SPI::device<pin_ctrl> _spi;
	int16_t _last[4];
};

#endif
