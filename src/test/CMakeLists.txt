set(
    TESTS 
        "main.cc"
        "gpio_test.cc"
        "ads1118_test.cc"
)
add_executable(tests ${TESTS})
target_include_directories(tests PUBLIC "..")
target_link_libraries(tests rpi gtest gmock fmt bcm2835)
