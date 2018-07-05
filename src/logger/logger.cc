#include "HYT221.hh"
#include "BMP280.hh"
#include "zs042.hh"
#include "ads1118.hh"
#include "alphasense.hh"
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
    ads1118 adc1(5);//direccion ADC 1 (chipselect)
    ads1118 adc2(19);
    ads1118 adc3(6);
    ads1118 adc4(13);	
    std::ofstream f("log.txt");
   	
    for(;;) {
	using namespace std::chrono_literals;
	hyt221::measurement m = sensor_ht.read_h_t();
	ZS042::measurement cl =f_clock.clock_read();
	float p = sensor_p.read_p();	
	int16_t cv1= adc1.in(3); //ALPHASENSE NO2 VREF
 	std::this_thread::sleep_for(0.1s);
	int16_t cv2= adc1.in(0); // ALPHASENSE NO2 AUX
	std::this_thread::sleep_for(0.1s);
        int16_t cv3= adc2.in(3); //ALPHASENSE O3 VREF
 	std::this_thread::sleep_for(0.1s);
	int16_t cv4= adc2.in(0); // ALPHASENSE O3 AUX
        std::this_thread::sleep_for(0.1s);
	int16_t cv5= adc3.in(3); //SPEC O3 VREF
        std::this_thread::sleep_for(0.1s);
        int16_t cv6= adc3.in(0); //SPEC O3 VAUX
        std::this_thread::sleep_for(0.1s);
        int16_t cv7= adc4.in(3); // SPEC NO2 VREF 
        std::this_thread::sleep_for(0.1s);
        int16_t cv8= adc4.in(0);// SPEC NO2 VAUX
	double alphano2vref= conversion(cv1);
	double alphano2aux= conversion(cv2);
	double alphao3vref= conversion(cv3);
        double alphao3aux= conversion(cv4);
        double speco3vref= conversion (cv5);
        double speco3aux= conversion (cv6);
        double specno2vref= conversion (cv7);
        double specno2aux= conversion (cv8);
	std::time_t now = std::time(nullptr);
	char ts[80];

	f << cl.year <<"-"<<cl.month
		  <<"-"<<cl.date<<" "<<cl.hours
		  <<":"<<cl.minutes<<":"<<cl.seconds
	          <<" "<< m.t << " "<< m.h 
                  <<" "<<alphano2vref
                  <<" "<<alphano2aux
                  <<" "<<alphao3vref
                  <<" "<<alphao3aux
                  <<" "<<speco3vref
                  <<" "<<speco3aux
                  <<" "<<specno2vref
                  <<" "<<specno2aux
                  <<" "<< p/100.f 
                  <<std::endl;
	std::cout << cl.year <<"-"<<cl.month
		  <<"-"<<cl.date<<" "<<cl.hours
		  <<":"<<cl.minutes<<":"<<cl.seconds
	          <<" "<<"T=" << m.t <<"ºC" << " "
		  << "RH=" << m.h <<"%"<< " "
                  <<" "<<"adc1_1="<<alphano2vref
                  <<" "<<"adc1_2="<<alphano2aux
                  <<" "<<"adc2_1="<<alphao3vref
                  <<" "<<"adc2_2="<<alphao3aux
                  <<" "<<"adc3_1="<<speco3vref
                  <<" "<<"adc3_2="<<speco3aux
                  <<" "<<"adc4_1="<<specno2vref
                  <<" "<<"adc4_2="<<specno2aux
                  <<" "<< p/100.f << "Pa"<< 
                  std::endl;
	f.flush();
	std::this_thread::sleep_for(3s);
	
    }
};
