#include "ads1118.hh"
#include "gmock/gmock.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

class FILETest: public testing::Test {
public:
  FILETest() {}
};

TEST_F(FILETest, TestFILEOperations) {
    FILE *archivo = fopen("/home/pi/TFG/EABC/src/test/prueba1/prueba1.dat","w+b");
    ofstream fichero("datos.dat",ios::in| ios::out | ios::binary);//creado objeto de la clase ofstream
    if (fichero.is_open()){    // verifico que le haya abierto, y le cierro
      printf ("el archivo se ha abierto con exito\n");
      int s = 12345;
      fichero.write(reinterpret_cast<char*>(&s), sizeof(s));// el programa compila perfecto, pero no veo que escriba en el fichero.
      fichero.close();
    }
    else{ // fichero no se ha podido abrir
       printf ("el archivo no se ha podido abrir\n");   
    }
}
