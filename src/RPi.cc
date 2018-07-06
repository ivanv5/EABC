#include "I2C.hh"
#include "SPI.hh"

RPi::system RPi::base::_rpi;
I2C::bus I2C::device::_bus;

template<>
SPI::bus<RPi::spi_ctrl> SPI::device<RPi::spi_ctrl>::_bus = { 5, 19, 6, 13 };

const uint8_t SPI::cs_ctrl::_cspins[3] = {
	BCM2835_SPI_CS0, BCM2835_SPI_CS1, BCM2835_SPI_CS2
};

void I2C::check_reason_code(uint8_t code)
{
	if (code == BCM2835_I2C_REASON_OK) return;

	if (code == BCM2835_I2C_REASON_ERROR_NACK)
		throw std::runtime_error("Received a NACK");

	if (code == BCM2835_I2C_REASON_ERROR_CLKT)
		throw std::runtime_error("Received Clock Stretch Timeout");

	if (code == BCM2835_I2C_REASON_ERROR_DATA)
		throw std::runtime_error("Not all data is sent / received");
}
