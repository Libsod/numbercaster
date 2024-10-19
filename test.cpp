#include "app.h" // Adjust this based on your actual class declarations
#include <gtest/gtest.h>
#include <sstream> // For std::istringstream

class NumberCastTest : public testing::Test {
  protected:
    NumberCast numberCast;
};

// Test valid input handling
TEST_F(NumberCastTest, ValidInput) {
  std::istringstream input("42.0"); // Mock input
  std::cin.rdbuf(input.rdbuf());    // Redirect cin

  auto result = numberCast.getInput("Enter a number: ");
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 42.0); // Check if the returned value is as expected
}

// Test invalid input handling
TEST_F(NumberCastTest, InvalidInput) {
  std::istringstream input("invalid"); // Mock invalid input
  std::cin.rdbuf(input.rdbuf());       // Redirect cin

  auto result = numberCast.getInput("Enter a number: ");
  EXPECT_FALSE(
      result.has_value()); // Check that no value is returned for invalid input
}

// Test bounds checking within limits
TEST_F(NumberCastTest, WithinBounds) {
  f64 value = 10.0;
  EXPECT_NO_THROW(numberCast.checkBounds(value)); // Should not throw
}

// Test bounds checking below lower limit
TEST_F(NumberCastTest, BelowLowerBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::min()) - 1;
  EXPECT_THROW(numberCast.checkBounds(value),
               std::out_of_range); // Should throw
}

// Test bounds checking above upper limit
TEST_F(NumberCastTest, AboveUpperBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::max()) + 1;
  EXPECT_THROW(numberCast.checkBounds(value),
               std::out_of_range); // Should throw
}

// Test casting to i32
TEST_F(NumberCastTest, CastingToI32) {
  f64 value = 42.0;
  EXPECT_EQ(numberCast.castToI32(value), 42); // Test casting to i32
}

// Test getting the maximum value
TEST_F(NumberCastTest, GetMaxPositive) {
  EXPECT_EQ(numberCast.getMax(1, 2), 2);
}

TEST_F(NumberCastTest, GetMaxNegative) {
  EXPECT_EQ(numberCast.getMax(-1, -2), -1);
}

TEST_F(NumberCastTest, GetMaxZero) { EXPECT_EQ(numberCast.getMax(0, 0), 0); }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
