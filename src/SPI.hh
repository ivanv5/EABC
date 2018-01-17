#ifndef SPI_HH
#define SPI_HH
# include <bcm2835.h>
# include <algorithm>
# include <system_error>

namespace SPI {

    class bus {
    public:
	bus() : _current(0) {
	    if (!bcm2835_spi_begin())
		throw std::runtime_error("Failed to init SPI. Are you root?");
	    auto div = bcm2835_aux_spi_CalcClockDivider(4000000);
	    bcm2835_aux_spi_setClockDivider(div);
	    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	}
	
	~bus() { bcm2835_spi_end(); }

    };
    
    class device {
    public:
	explicit device(uint8_t address) : _address(address) {}
	
	template <class T>
	void read(T& data) const {
	    xfer(data)
	}

	template <class T>
	void write(const T& data) const {
	    xfer(data, T());
	}

	template <class T>
	void xfer(T& data) const {
	    char* buf = reinterpret_cast<char>(data);
	    bcm2835_spi_transfern(buf, sizeof(T));
	}

	template <class T, class U>
	void xfer(const T& data_out, U& data_in) const {
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
