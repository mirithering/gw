#include "random.h"

#include <bits/stdc++.h>

namespace {
std::optional<int> g_random_override = std::nullopt;
std::optional<Percent> g_chance_override = std::nullopt;

bool g_random_deterministic = true;
std::default_random_engine g_deterministic_random_engine(1337);

int RandomImpl(int minimum, int maximum) {
  if (g_random_deterministic) {
    std::uniform_int_distribution<int> uniform_dist(minimum, maximum);
    return uniform_dist(g_deterministic_random_engine);
  }

  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(minimum, maximum);
  return uniform_dist(e1);
}

int Random(int minimum = 0, int maximum = 99) {
  if (g_random_override) {
    int override = g_random_override.value();
    g_random_override = std::nullopt;
    return override;
  }
  return RandomImpl(minimum, maximum);
}

Percent RandomPercent() {
  if (g_chance_override) {
    Percent override = g_chance_override.value();
    g_chance_override = std::nullopt;
    return override;
  }
  return Percent(RandomImpl(0, 99));
}
}  // namespace

bool RandomDecision(Percent chance_for_true) {
  return ::RandomPercent() < chance_for_true;
}

int RandomValue(int minimum, int maximum) { return ::Random(minimum, maximum); }

void OverrideRandomValueForTesting(int roll_result) {
  g_random_override = roll_result;
}

void OverrideRandomDecisionForTesting(
    Percent everythinig_greater_than_this_will_be_true) {
  g_chance_override = everythinig_greater_than_this_will_be_true;
}

void SetDeterministicRandom(bool deterministic) {
  g_random_deterministic = deterministic;
}