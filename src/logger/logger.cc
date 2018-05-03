#include "HYT221.hh"
#include <fstream>
#include <iostream>
#include <ctime>
#include <locale>
#include <thread>
#include <cerrno>

int main() {
    hyt221 sensor;
    std::ofstream f("log.txt");

    for(;;) {
	using namespace std::chrono_literals;
	hyt221::measurement m = sensor.read_t_h();
	std::time_t now = std::time(nullptr);
	char ts[80];
	std::strftime(ts, sizeof(ts), "%Y%m%d%H%M%S", std::localtime(&now));
	f << ts << " " << m.t << " " << m.h << std::endl;
	std::cout << ts << " " << m.t << " " << m.h << std::endl;
	f.flush();
	std::this_thread::sleep_for(5s);
    }
}
