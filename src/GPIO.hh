#ifndef GPIO_HH
#define GPIO_HH
#include <iostream>

namespace GPIO {

    // RAII class for export/unexport
    class gpio_pin {
	int _pin;

	explicit gpio_pin(int pin);
	~gpio_pin();

	friend class gpio;
	friend std::ostream& operator<< (std::ostream& os, const gpio_pin& pin) {
	    return os << pin._pin;
	}
    };

    // User-level interface
    class gpio {
    public:
	explicit gpio(int pin);
	~gpio();

	void set(bool v);
	bool get() const;
    private:
	gpio_pin _pin; 
    };
    
}

#endif
