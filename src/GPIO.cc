#include "GPIO.hh"

#include <string>
#include <fstream>
#include <stdexcept>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace GPIO;

//const std::string sysfs_path{"/sys/class/gpio"};
const std::string sysfs_path{"."};

gpio::gpio(int pin) : _pin(pin)
{
}

gpio::~gpio()
{
}

void gpio::set(bool v)
{
    std::ofstream sysfs(fmt::format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(fmt::format("Unable to set gpio{}", _pin));
    sysfs << (int)v;
}

bool gpio::get() const
{
    std::ifstream sysfs(fmt::format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(fmt::format("Unable to get gpio{}", _pin));
    bool v;
    sysfs >> v;
    return v;
}

gpio_pin::gpio_pin(int pin) : _pin(pin)
{
    std::ofstream sysfs(fmt::format("{}/export", sysfs_path));
    if(!sysfs.is_open())
	throw std::runtime_error(fmt::format("Unable to export GPIO {}", _pin));
    sysfs << pin;
}

gpio_pin::~gpio_pin()
{
    std::ofstream sysfs(fmt::format("{}/unexport", sysfs_path));
    if(sysfs.is_open())
	sysfs << _pin;
}
