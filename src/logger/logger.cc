#include "HYT221.hh"
#include "BMP280.hh"
#include "zs042.hh"
#include "ads1118.hh"
#include <fstream>
#include <iostream>
#include <ctime>
#include <locale>
#include <thread>
#include <cerrno>

class differential_ad {
public:
	differential_ad(uint8_t addr) : _ad(addr) {}
	double vref() { return _ad.in_V(3); }
	double vaux() { return _ad.in_V(0); }
private:
	ads1118<> _ad;
};

class single_ended_ad {
public:
	single_ended_ad(uint8_t addr) : _ad(addr) {}
	double vref() { return _ad.in_V(3); }
	double vaux() { return _ad.in_V(0); }
private:
	ads1118<> _ad;
};

template <class ad_type>
struct gas_sensor {
	gas_sensor(uint8_t addr) : no2(addr), o3(addr + 1) {}

	ad_type no2;
	ad_type o3;
};

using alpha_sense = gas_sensor<differential_ad>;
using spec_sensor = gas_sensor<differential_ad>;

std::string fmt_rtc(ZS042::measurement clk) {
	return fmt::format("{}-{}-{} {}:{}:{}",
					clk.year, clk.month, clk.date,
					clk.hours, clk.minutes, clk.seconds)
}

template <class sensor_set>
std::string fmt_sensor_set(sensor_set set) {
	return fmt::format("{} {}", 
					fmt_sensor(set.no2),
					fmt_sensor(set.o3));
}

template <class sensor>
std::string fmt_sensor(sensor s) {
	return fmt::format("{} {}", s.vref(), s.vaux());
}

int main() 
{
	hyt221 hyt;
	bmp280 p;
	ZS042 rtc;
	alpha_sense alpha(0);
	spec_sensor spec(2);
	std::ofstream f("log.txt");

	for(;;) {
		using namespace std::chrono_literals;
		auto hyt = ht.read_h_t();
		auto clk = rtc.clock_read();

		std::string log_record = 
			fmt::format("{} {} {} {} {} {}",
						fmt_rtc(clk), hyt.t, hyt.h, 
						p.read_p() / 100.f,
						fmt_sensor_set(alpha),
						fmt_sensor_set(spec));
		
		f << log_record << std::endl;
		f.flush();
		std::cout << log_record << std::endl;
		std::this_thread::sleep_for(3s);	
	}
}
