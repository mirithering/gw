#include "units.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

TEST(UnitsTest, SecondIs1000) { ASSERT_EQ(Second.milliseconds(), 1000); }
TEST(UnitsTest, MillisecondIs1) { ASSERT_EQ(Millisecond.milliseconds(), 1); }

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