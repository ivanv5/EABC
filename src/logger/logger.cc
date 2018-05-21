#include "HYT221.hh"
#include "BMP280.hh"
#include "zs042.hh"
#include <fstream>
#include <iostream>
#include <ctime>
#include <locale>
#include <thread>
#include <cerrno>

int main() {
    hyt221 sensor_ht;
    bmp280 sensor_p;
    ZS042 f_clock;
    std::ofstream f("log.txt");

    for(;;) {
	using namespace std::chrono_literals;
	hyt221::measurement m = sensor_ht.read_h_t();
	ZS042::measurement cl =f_clock.clock_read();
	float p = sensor_p.read_p();
	std::time_t now = std::time(nullptr);
	char ts[80];
	// std::strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	f << cl.year <<"-"<<cl.month
		  <<"-"<<cl.date<<" "<<cl.hours
		  <<":"<<cl.minutes<<":"<<cl.seconds
	          << " " << m.t << " " << m.h << " "
	          << p/100.f<< std::endl;
	std::cout << cl.year <<"-"<<cl.month
		  <<"-"<<cl.date<<" "<<cl.hours
		  <<":"<<cl.minutes<<":"<<cl.seconds
	          <<" "<<"T=" << m.t <<"ºC" << " "
		  << "RH=" << m.h <<"%"<< " "
		  << p/100.f << "Pa"<< std::endl;
	f.flush();
	std::this_thread::sleep_for(5s);
	
    }
};
