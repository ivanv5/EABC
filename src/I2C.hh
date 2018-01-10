#ifndef I2C_HH
#define I2C_HH
# include <bcm2835.h>

namespace I2C {

    void check_reason_code(uint8_t code);

    class bus {
	uint8_t _current;
	
    public:
	bus() : _current(0) {
	    if (!bcm2835_i2c_begin())
		throw std::runtime_error("Failed to init I2C. Are you root?");
	    bcm2835_i2c_set_baudrate(100000);
	}
	
	~bus() { bcm2835_i2c_end(); }

	void set_address(uint8_t addr) {
	    if (_current == addr) return;
	    _current = addr;
	    bcm2835_i2c_setSlaveAddress(slave);
	}
    };
    
    class device {
    public:
	explicit device(uint8_t address) : _address(address) {}
	
	template <class T>
	void read(T& data) const {
	    _bus.set_address(_address);
	    char* buf = reinterpret_cast<char>(data);
	    check_reason_code(bcm2835_i2c_read(buf, sizeof(T)));
	}

	template <class T>
	void write(const T& data) const {
	    _bus.set_address(_address);
	    char* buf = reinterpret_cast<char>(data);
	    check_reason_code(bcm2835_i2c_write(buf, sizeof(T)));
	}

	template <class W, class R>
	void write_read_rs(const W& cmd, R& data) const {
	    _bus.set_address(_address);
	    char* wbuf = reinterpret_cast<char>(cmd);
	    char* rbuf = reinterpret_cast<char>(data);
	    check_reason_code(bcm2835_i2c_write_read_rs(wbuf, sizeof(W), rbuf, sizeof(R)));
	}

    private:
	static bus _bus;
    };

}

#endif
