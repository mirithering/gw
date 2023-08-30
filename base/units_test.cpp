#include "units.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

namespace {
const double epsilon = 1.0e-10;
}

TEST(UnitsTest, SecondIs1000) { ASSERT_EQ(Second.value(), 1000); }
TEST(UnitsTest, MillisecondIs1) { ASSERT_EQ(Millisecond.value(), 1); }

TEST(UnitsTest, MultiplicationCommutative) {
  ASSERT_EQ(Second * 25, 25 * Second);
}

TEST(UnitsTest, MultiplicationAssociativeLeft) {
  ASSERT_EQ(Second * (25 * 2), (Second * 25) * 2);
}

TEST(UnitsTest, MultiplicationAssociativeRight) {
  ASSERT_EQ(56 * (25 * Second), (56 * 25) * Second);
}

TEST(UnitsTest, MultiplicationUnit) { ASSERT_EQ(10 * Second * 1, 10 * Second); }

TEST(UnitsTest, PercentOfInteger) { ASSERT_EQ(of(10, Percent(10)), 1); }

TEST(UnitsTest, PercentOfDouble) {
  ASSERT_TRUE(std::abs(of(30.0, Percent(100.0 / 3.0)) - 10.0) < epsilon);
}