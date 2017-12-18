set(
    TESTS 
        "main.cc"
        "gpio_test.cc"
)
add_executable(tests ${TESTS})
target_link_libraries(tests gpio gtest gmock)
