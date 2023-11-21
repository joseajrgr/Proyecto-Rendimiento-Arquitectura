# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in sim

# Configura la opción de compilación para desactivar el color en Google Test
add_compile_options(-DGTEST_COLOR_NO)
add_definitions(-DGTEST_COLOR_NO)

add_executable(ftest
    fluid_test.cpp)

# Library dependencies
target_link_libraries (ftest
        PRIVATE
        GTest::gtest_main
        Microsoft.GSL::GSL)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(ftest)