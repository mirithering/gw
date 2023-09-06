#include "units.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "logging.h"

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

TEST(UnitsTest, DistanceLine) {
  Position p1 = {Inches(0), Inches(10)};
  Position p2 = {Inches(0), Inches(-10)};
  ASSERT_EQ(Distance(p1, p2), Inches(20));
}

TEST(UnitsTest, DistanceComplex) {
  Position p1 = {Inches(5), Inches(6)};
  Position p2 = {Inches(3), Inches(7)};
  ASSERT_TRUE(Distance(p1, p2) - Inches(2.23607) < Inches(0.0001));
}

TEST(UnitsTest, InRange) {
  Position p1 = {Inches(5), Inches(3)};
  Position p2 = {Inches(3), Inches(7)};
  ASSERT_TRUE(InRange(p1, p2, Inches(4.5)));
  ASSERT_FALSE(InRange(p1, p2, Inches(4.3)));
}

TEST(UnitsTest, MoveHorizontally) {
  Position p = {Inches(1), Inches(2)};
  Direction d = {Inches(1), Inches(0)};
  Speed s(Inches(1));
  Position next = {Inches(2), Inches(2)};
  ASSERT_EQ(NextPosition(p, d, s), next);
}

TEST(UnitsTest, MoveVertically) {
  Position p = {Inches(1), Inches(2)};
  Direction d = {Inches(0), Inches(1)};
  Speed s(Inches(1));
  Position next = {Inches(1), Inches(3)};
  ASSERT_EQ(NextPosition(p, d, s), next);
}

TEST(UnitsTest, MovingOutOfAggroCircleTakes4SecondsDiagonally) {
  Position p = {Inches(0), Inches(0)};
  Position initial = p;
  const Direction d = {Inches(1), Inches(1)};
  const Speed s = WalkingSpeed;
  for (int i = 0; Time(i) <= 4 * Second; ++i) {
    ASSERT_TRUE(InRange(initial, p, EarshotRange));
    p = NextPosition(p, d, s);
  }
  ASSERT_FALSE(InRange(initial, p, EarshotRange));
}

// This takes a millisecond longer because my value for speed is not a nice
// number.
TEST(UnitsTest, MovingOutOfAggroCircleTakesRoughly4SecondsWithRandomDirection) {
  Position p = {Inches(0), Inches(0)};
  Position initial = p;
  const Direction d = {Inches(153), Inches(185)};
  const Speed s = WalkingSpeed;
  for (int i = 0; Time(i) <= 4 * Second - Time(1); ++i) {
    ASSERT_TRUE(InRange(initial, p, EarshotRange)) << Distance(initial, p);
    p = NextPosition(p, d, s);
  }
  ASSERT_FALSE(InRange(initial, p, EarshotRange)) << Distance(initial, p);
}