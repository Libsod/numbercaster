#include "app.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(NumberCastTest, ValidInput) {
  std::istringstream input("42.0");
  std::cin.rdbuf(input.rdbuf());

  auto result = NumberCast::getInput("Enter a number: ");
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 42.0);
}

TEST(NumberCastTest, InvalidInput) {
  std::istringstream input("invalid");
  std::cin.rdbuf(input.rdbuf());

  auto result = NumberCast::getInput("Enter a number: ");
  EXPECT_FALSE(result.has_value());
}

TEST(NumberCastTest, WithinBounds) {
  f64 value = 10.0;
  EXPECT_NO_THROW(NumberCast::checkBounds(value));
}

TEST(NumberCastTest, BelowLowerBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::min()) - 1;
  EXPECT_THROW(NumberCast::checkBounds(value), std::out_of_range);
}

TEST(NumberCastTest, AboveUpperBound) {
  f64 value = static_cast<f64>(std::numeric_limits<i32>::max()) + 1;
  EXPECT_THROW(NumberCast::checkBounds(value), std::out_of_range);
}

TEST(NumberCastTest, CastingToI32) {
  f64 value = 42.0;
  EXPECT_EQ(static_cast<i32>(value), 42);
}

TEST(NumberCastTest, GetMaxPositive) { EXPECT_EQ(NumberCast::getMax(1, 2), 2); }

TEST(NumberCastTest, GetMaxNegative) {
  EXPECT_EQ(NumberCast::getMax(-1, -2), -1);
}

TEST(NumberCastTest, GetMaxZero) { EXPECT_EQ(NumberCast::getMax(0, 0), 0); }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
