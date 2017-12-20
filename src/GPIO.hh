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
	enum class direction {
	    IN,
	    OUT
	};
	
	enum class edge {
	    NONE,
	    RAISING,
	    FALLING,
	    BOTH
	};

	explicit gpio(int pin,
		      direction d = direction::IN,
		      edge e = edge::NONE,
		      bool active_low = false);
	~gpio();

	void set(bool v);
	bool get() const;
	void configure(direction);
	void configure(edge);
	void set_active_low(bool v = true);
    private:
	gpio_pin _pin; 
    };
    
}

#endif
