#include <gtest/gtest.h>
#include "sim/block.cpp"

TEST(Block_Tests, El_Nano) {
    // Arrange
    int result;
    // Act
    result = 33;

    // Assert
    ASSERT_EQ(result, 33);
}

TEST(Block_Tests, Not_El_Nano) {
    // Arrange
    int result;
    // Act
    result = 32;

    // Assert
    ASSERT_NE(result, 33);
}