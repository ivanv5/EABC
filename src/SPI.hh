#ifndef SPI_HH
#define SPI_HH
# include "RPi.hh"
# include <algorithm>

namespace SPI {

  enum class speed {
    Pi3_200Mhz         = 1U << 1,
    Pi3_100Mhz         = 1U << 2,
    Pi3_50Mhz          = 1U << 3,
    Pi3_25Mhz          = 1U << 4,
    Pi3_12_5Mhz        = 1U << 5,
    Pi3_6_25Mhz        = 1U << 6,
    Pi3_3_125Mhz       = 1U << 7,
    Pi3_1_5625Mhz      = 1U << 8,
    Pi3_781_25Khz      = 1U << 9,
    Pi3_390_625Khz     = 1U << 10,
    Pi3_195_3125Khz    = 1U << 11,
    Pi3_97_65625Khz    = 1U << 12,
    Pi3_48_828125Khz   = 1U << 13,
    Pi3_24_4140625Khz  = 1U << 14,
    Pi3_12_20703125Khz = 1U << 15,
    Pi3_6_1035156Khz   = 1U << 16,
  };
  
  class bus : public RPi::base {
	uint8_t _addr;
	speed _speed;

    public:
	bus() : _addr(~0) {
	    if (!bcm2835_spi_begin())
		throw std::runtime_error("Failed to init SPI. Are you root?");
	    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS2, 0);
	    set_address(0);
	    set_speed(Pi3_3_125Mhz);
	}
	
	~bus() { bcm2835_spi_end(); }

	void set_address(uint8_t addr) {
	    if (_addr == addr) return;
	    _addr = addr;
	    bcm2835_spi_chipSelect(addr);
	}

	void set_speed(speed s) {
	    if (_speed == s) return;
	    _speed = s;
	    bcm2835_spi_setClockDivider(s);
	}

    };
    
    class device {
    public:
	explicit device(uint8_t address) : _address(address) {}
	
	template <class T>
	void read(T& data) const { xfer(data); }

	template <class T>
	void write(const T& data) const { T out; xfer(data, out); }

	template <class T>
	void xfer(T& data) const { xfer(data, data); }

	template <class T, class U>
	void xfer(const T& data_out, U& data_in) const {
	    _bus.set_address(_address);
	    char& in  = reinterpret_cast<char&>(data_in);
	    char& out  = const_cast<char&>(reinterpret_cast<const char&>(data_out));
	    bcm2835_spi_transfernb(&out, &in,
				   std::max(sizeof(T), sizeof(U)));
	}

    private:
	uint8_t _address;
	static bus _bus;
    };

}

#endif
