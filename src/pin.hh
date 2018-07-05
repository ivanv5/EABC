#include <wiringPi.h>
#include <unistd.h>

void activepin() {
wiringPiSetupGpio();
pinMode(5,OUTPUT);
digitalWrite(5,1);

};