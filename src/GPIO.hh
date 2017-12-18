#ifndef GPIO_HH
#define GPIO_HH

namespace GPIO {

  struct Exporter {
    Exporter(int pin);
    ~Exporter();
  };

}

#endif
