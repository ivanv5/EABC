#include "HYT221.hh"
#include "BMP280.hh"
#include <fstream>
#include <iostream>
#include <ctime>
#include <locale>
#include <thread>
#include <cerrno>

int main() {
    hyt221 sensor_th;
    bmp280 sensor_p;
    std::ofstream f("log.txt");

    for(;;) {
	using namespace std::chrono_literals;
	hyt221::measurement m = sensor_th.read_t_h();
	float p = sensor_p.read_p();
	std::time_t now = std::time(nullptr);
	char ts[80];
	std::strftime(ts, sizeof(ts), "%Y%m%d%H%M%S", std::localtime(&now));
	f << ts << " " << m.t << " " << m.h << " " << p << std::endl;
	std::cout << ts << " " << m.t << " " << m.h << " " << p << std::endl;
	f.flush();
	std::this_thread::sleep_for(5s);
    }
}
