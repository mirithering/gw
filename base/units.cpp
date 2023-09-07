#include "units.h"

#include <bits/stdc++.h>

#include "logging.h"

Inches Distance(const Position& p1, const Position& p2) {
  double dx = std::abs((p1.first - p2.first).value());
  double dy = std::abs((p1.second - p2.second).value());
  return Inches(std::sqrt(dx * dx + dy * dy));
}

bool InRange(const Position& p1, const Position& p2, const Inches& range) {
  return Distance(p1, p2) <= range;
}

Direction Towards(const Position& from, const Position& to) {
  return {to.first - from.first, to.second - from.second};
}

// Calculates the position after one tick if moving with speed in direction.
Position NextPosition(const Position& current, const Direction& direction,
                      const Speed& speed) {
  Inches distance_covered_by_direction =
      Distance({Inches(0), Inches(0)}, direction);
  double normalize_factor =
      Inches(speed.value()) / distance_covered_by_direction;
  return {current.first + normalize_factor * direction.first,
          current.second + normalize_factor * direction.second};
}

Time operator/(Inches lhs, Speed rhs) {
  return Time(lhs.value() / rhs.value().value());
}