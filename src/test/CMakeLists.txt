set(
    TESTS 
        "main.cc"
        "gpio_test.cc"
       
        "hyt221_test.cc"
        "file_test.cc"
)
add_executable(tests ${TESTS})
target_include_directories(tests PUBLIC "..")
target_link_libraries(tests rpi gtest gmock fmt bcm2835)
