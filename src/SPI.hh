#ifndef SPI_HH
#define SPI_HH
# include "RPi.hh"
# include <algorithm>
# include <stdio.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <locale>
#include <thread>
#include <cerrno>
#include <pigpio.h>
//
namespace SPI {
  
  class bus : public RPi::base {
	uint8_t _addr;
	unsigned _speed;

    public:
	bus():_addr(~0)   {
	    if (!bcm2835_spi_begin())
		throw std::runtime_error("Failed to init SPI. Are you root?");
	    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
	    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS2, 0);
	  //  set_address(_addr);
	    bcm2835_gpio_fsel(5,BCM2835_GPIO_FSEL_OUTP);
            bcm2835_gpio_fsel(6,BCM2835_GPIO_FSEL_OUTP);
            bcm2835_gpio_fsel(13,BCM2835_GPIO_FSEL_OUTP);
  	    bcm2835_gpio_fsel(19,BCM2835_GPIO_FSEL_OUTP);
	    set_speed(4000000);

	}
	
	~bus() { bcm2835_spi_end(); }

	void set_address(uint8_t addr) {
	    //if (_addr == addr) return;
	    _addr = addr;
	    //if(_addr==0){
	bcm2835_gpio_write(5,HIGH);
	bcm2835_gpio_write(6,HIGH);
	bcm2835_gpio_write(13,HIGH);
        bcm2835_gpio_write(19,HIGH);
	bcm2835_gpio_write(_addr,LOW);

	}

	void set_speed(unsigned s) {
	    if (_speed == s) return;
	    _speed = s;
	    uint16_t div = static_cast<uint16_t>(RPi::SYSTEM_CLOCK_FREQ/s);
	    // Divider must be even
	    bcm2835_spi_setClockDivider(div & 1);
	}

    };
    
    class device {
    public:
	explicit device(uint8_t address) : _address(address) {

}

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
	bcm2835_gpio_write(_address,HIGH);
}

    private:
	uint8_t _address;
	static bus _bus;
    };

}

#endif
