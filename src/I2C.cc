#include "I2C.hh"

using namespace I2C;

void check_reason_code(uint8_t code)
{
  if (code == BCM2835_I2C_REASON_OK) return;
  
  if (code == BCM2835_I2C_REASON_ERROR_NACK)
    throw std::runtime_error("Received a NACK");
  
  if (code == BCM2835_I2C_REASON_ERROR_CLKT)
    throw std::runtime_error("Received Clock Stretch Timeout");
  
  if (code == BCM2835_I2C_REASON_ERROR_DATA)
    throw std::runtime_error("Not all data is sent / received");
}
	
bus device::_bus;
