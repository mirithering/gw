#include "random.h"

#include <bits/stdc++.h>

namespace {
std::optional<int> g_random_override = std::nullopt;

bool g_random_deterministic = true;
std::default_random_engine g_deterministic_random_engine(1337);

int Random(int minimum = 0, int maximum = 99) {
  if (g_random_override) {
    int override = g_random_override.value();
    g_random_override = std::nullopt;
    return override;
  }

  if (g_random_deterministic) {
    std::uniform_int_distribution<int> uniform_dist(minimum, maximum);
    return uniform_dist(g_deterministic_random_engine);
  }

  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(minimum, maximum);
  return uniform_dist(e1);
}
}  // namespace

bool RandomDecision(Chance chance_for_true) {
  return ::Random() < chance_for_true.GetPercentage();
}

int RandomValue(int minimum, int maximum) { return ::Random(minimum, maximum); }

void OverrideRandomRollForTesting(int roll_result) {
  g_random_override = roll_result;
}

void SetDeterministicRandom(bool deterministic) {
  g_random_deterministic = deterministic;
}