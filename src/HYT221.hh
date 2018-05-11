#ifndef HYT221_HH
#define HYT221_HH
# include "I2C.hh"
#include <thread>

class hyt221 : public I2C::device {
    struct measurement_internal {
	char data[4];
    };
  
public:
    struct measurement {
	double t;
	double h;
    };

    hyt221() : I2C::device(0x28) {}
    
    measurement read_t_h() const {
	using namespace std::chrono_literals;

	char readCmd = '0';
	write(readCmd);
	std::this_thread::sleep_for(60ms);
	
	measurement_internal m;
	read(m);
	measurement ret {
	    ((m.data[0] & 0x3f)<<8 | m.data[1]) * 100. / 0x3fff,
	    (m.data[2]<<8 | m.data[3]&0xfc) * 165. / 0xfffc - 40.
	};
	return ret;
    }
};

#endif
