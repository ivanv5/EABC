#ifndef SPI_HH
#define SPI_HH
# include <bcm2835.h>
# include <algorithm>
# include <system_error>

namespace SPI {

    class bus {
	uint8_t _addr;
	uint32_t _speed;

    public:
	bus() : _addr(~0) {
	    if (!bcm2835_spi_begin())
		throw std::runtime_error("Failed to init SPI. Are you root?");
	    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS2, 0);
	    set_address(0);
	    set_speed(4000000);
	}
	
	~bus() { bcm2835_spi_end(); }

	void set_address(uint8_t addr) {
	    if (_addr == addr) return;
	    _addr = addr;
	    bcm2835_spi_chipSelect(addr);
	}

	void set_speed(uint32_t speed) {
	    if (_speed == speed) return;
	    _speed = speed;
	    auto div = bcm2835_aux_spi_CalcClockDivider(speed);
	    bcm2835_aux_spi_setClockDivider(div);
	}

    };
    
    class device {
    public:
	explicit device(uint8_t address) : _address(address) {}
	
	template <class T>
	void read(T& data) const { xfer(data) }

	template <class T>
	void write(const T& data) const { xfer(data, T()); }

	template <class T>
	void xfer(T& data) const { xfer(data, data); }

	template <class T, class U>
	void xfer(const T& data_out, U& data_in) const {
	    _bus.set_address(_address);
	    char* in  = reinterpret_cast<char>(data_in);
	    char* out  = reinterpret_cast<char>(data_out);
	    bcm2835_spi_transfernb(out, in,
				   std::max(sizeof(T), sizeof(U)));
	}

    private:
	uint8_t _address;
	static bus _bus;
    };

}

#endif
