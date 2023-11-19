#include <gtest/gtest.h>
#include "./trazas.cpp"

TEST(Fluid_Tests, El_Nano777) {
    // Arrange
    const char* script = "./ftest/scripts/test1.sh";
    // Act
    int result = std::system(script);
    const std::string filename = "out.fld";
    readTraceFile(filename);
    // Assert
    ASSERT_EQ(result, 0);
}

TEST(Fluid_Tests, Not_El_Nano777) {
    // Arrange
    int result;
    // Act
    result = 32;
    // Assert
    ASSERT_NE(result, 33);
}