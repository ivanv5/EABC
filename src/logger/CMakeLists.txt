set(LOGGER_SOURCES
  "logger.cc"
  )

set(CMAKE_CXX_FLAGS "-std=gnu++1y")

add_executable(logger ${LOGGER_SOURCES})
target_include_directories(logger PUBLIC "..")
target_link_libraries(logger rpi bcm2835)
