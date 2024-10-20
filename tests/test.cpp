#include "app.hpp"       // Include for testing NumberCast functions
#include <gtest/gtest.h> // Include Google Test framework for unit testing
#include <sstream>       // Include the std::istringstream

/* Test case to validate correct input handling in getInput() function */
TEST(NumberCastTest, ValidInput) {
  std::istringstream input("42.0"); // Simulate input of "42.0"
  std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use the simulated input

  auto result =
      NumberCast::getInput("Enter a number: "); // Call the function to test
  EXPECT_TRUE(result.has_value()); // Assert that the result contains a value
  EXPECT_EQ(result.value(), 42.0); // Assert that the returned value is 42.0
}

/* Test case to handle invalid input for getInput() function */
TEST(NumberCastTest, InvalidInput) {
  std::istringstream input("invalid"); // Simulate invalid input
  std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use the simulated input

  auto result =
      NumberCast::getInput("Enter a number: "); // Call the function to test
  EXPECT_FALSE(result.has_value()); // Assert that no value is returned
}

/* Test case to verify that a valid number is within bounds for checkBounds() */
TEST(NumberCastTest, WithinBounds) {
  f64 value = 10.0; // Valid input within i32 bounds
  EXPECT_NO_THROW(
      NumberCast::checkBounds(value)); // Assert that no exception is thrown
}

/* Test case to check behavior for value below the lower bound of i32 */
TEST(NumberCastTest, BelowLowerBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::min()) -
              1; // Below minimum i32 value
  EXPECT_THROW(
      NumberCast::checkBounds(value),
      std::out_of_range); // Assert that an out_of_range exception is thrown
}

/* Test case to check behavior for value above the upper bound of i32 */
TEST(NumberCastTest, AboveUpperBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::max()) +
              1; // Above maximum i32 value
  EXPECT_THROW(
      NumberCast::checkBounds(value),
      std::out_of_range); // Assert that an out_of_range exception is thrown
}

/* Test case to validate proper casting from f64 to i32 */
TEST(NumberCastTest, CastingToI32) {
  f64 value = 42.0; // Define a floating-point number
  EXPECT_EQ(static_cast<i32>(value),
            42); // Assert that casting to i32 yields 42
}

/* Test case to check getMax() function with positive integers */
TEST(NumberCastTest, GetMaxPositive) {
  EXPECT_EQ(NumberCast::getMax(1, 2),
            2); // Assert that 2 is the maximum of 1 and 2
}

/* Test case to check getMax() function with negative integers */
TEST(NumberCastTest, GetMaxNegative) {
  EXPECT_EQ(NumberCast::getMax(-1, -2),
            -1); // Assert that -1 is the maximum of -1 and -2
}

/* Test case to check getMax() function with zero values */
TEST(NumberCastTest, GetMaxZero) {
  EXPECT_EQ(NumberCast::getMax(0, 0),
            0); // Assert that 0 is the maximum of two zeros
}

/* Main function to initialize and run all Google Test cases */
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv); // Initialize Google Test framework
  return RUN_ALL_TESTS();               // Run all the test cases
}
