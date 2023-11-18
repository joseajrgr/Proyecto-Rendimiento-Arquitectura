#include <gtest/gtest.h>
#include "sim/grid.cpp"
#include "fluid/fluid.cpp"

TEST(Fluid_Tests, El_Nano777) {
    // Arrange
    int result;
    // Act
    result = 33;
    // Assert
    ASSERT_EQ(result, 33);
}

TEST(Fluid_Tests, Not_El_Nano777) {
    // Arrange
    int result;
    // Act
    result = 32;

    // Assert
    ASSERT_NE(result, 33);
}