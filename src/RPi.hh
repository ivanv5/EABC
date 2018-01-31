#ifndef RPI_HH
#define RPI_HH
# include <bcm2835.h>
# include <system_error>

namespace RPi {

    struct system {
	system() {
	    if (!bcm2835_init())
		throw std::runtime_error("Failed to init BCM2835");
	}
	~system() { bcm2835_close(); }
    };

    const unsigned SYSTEM_CLOCK_FREQ = 400000000;
  
    class base {
	static system _rpi;
    };
}

#endif
