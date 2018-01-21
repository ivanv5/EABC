set(SOURCES
  "RPi.cc"
  "GPIO.cc"
  "GPIO.hh"
  "I2C.hh"
  "SPI.hh"
  "ads1118.hh"
)
set(CMAKE_CXX_FLAGS "-std=gnu++1y")


add_library(rpi ${SOURCES})
target_include_directories(rpi PUBLIC ".")
target_include_directories(rpi PUBLIC "../fmtlib")
#target_compile_features(rpi PUBLIC "cxx_std_17")
target_link_libraries(rpi)

install(
  TARGETS rpi
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib
)

add_subdirectory("test")
