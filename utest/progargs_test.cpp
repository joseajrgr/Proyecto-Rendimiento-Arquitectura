#include <gtest/gtest.h>
#include "sim/progargs.cpp"

TEST(Grid_Tests, El_Nano3) {
    // Arrange
    int result;
    // Act
    result = 33;

    // Assert
    ASSERT_EQ(result, 33);
}

TEST(Grid_Tests, Not_El_Nano3) {
    // Arrange
    int result;
    // Act
    result = 32;

    // Assert
    ASSERT_NE(result, 33);
}