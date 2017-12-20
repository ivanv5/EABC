#include "GPIO.hh"

#include <string>
#include <functional>
#include <fstream>
#include <stdexcept>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <unistd.h> // usleep

using namespace GPIO;
using namespace fmt;

//const std::string sysfs_path{"/sys/class/gpio"};
const std::string sysfs_path{"."};

constexpr int RETRY = 10000; // us
constexpr int TIMEOUT = 2000000 / RETRY; // 2s

static void
retry(const std::function<void()>& f)
{
    for(int i=0; i<TIMEOUT; ++i) {
	try {
	    f();
	    return;
	}
	catch(...) {
	}
	usleep(10000);
    }
    f();
}

gpio::gpio(int pin, direction d, edge e, bool active_low)
    : _pin(pin)
{
    retry([=] {
	    configure(d);
	    configure(e);
	    set_active_low(active_low);
	});
}

gpio::~gpio()
{
}

void gpio::configure(direction d)
{
    std::ofstream sysfs(format("{}/gpio{}/direction", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to set direction of gpio{}", _pin));

    const char* dir_names[] = {
	"in",
	"out"
    };
    sysfs << dir_names[static_cast<int>(d)];
}

void gpio::configure(edge e)
{
    std::ofstream sysfs(format("{}/gpio{}/edge", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to set active edge of gpio{}", _pin));

    const char* edge_names[] = {
	"none",
	"raising",
	"falling",
	"both"
    };
    sysfs << edge_names[static_cast<int>(e)];
}

void gpio::set_active_low(bool v)
{
    std::ofstream sysfs(format("{}/gpio{}/active_low", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to set active_low gpio{}", _pin));
    sysfs << (int)v;
}

void gpio::set(bool v)
{
    std::ofstream sysfs(format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to set gpio{}", _pin));
    sysfs << (int)v;
}

bool gpio::get() const
{
    std::ifstream sysfs(format("{}/gpio{}/value", sysfs_path, _pin));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to get gpio{}", _pin));
    bool v;
    sysfs >> v;
    return v;
}

gpio_pin::gpio_pin(int pin) : _pin(pin)
{
    std::ofstream sysfs(format("{}/export", sysfs_path));
    if(!sysfs.is_open())
	throw std::runtime_error(format("Unable to export GPIO {}", _pin));
    sysfs << pin;
}

gpio_pin::~gpio_pin()
{
    std::ofstream sysfs(format("{}/unexport", sysfs_path));
    if(sysfs.is_open())
	sysfs << _pin;
}
