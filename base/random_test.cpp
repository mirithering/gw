#include "random.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

TEST(RandomTest, OverrideToNoPercent) {
  OverrideRandomDecisionForTesting(Percent(99));
  ASSERT_TRUE(RandomDecision(Percent(100)));

  OverrideRandomDecisionForTesting(Percent(99));
  ASSERT_FALSE(RandomDecision(Percent(99)));

  OverrideRandomDecisionForTesting(Percent(99));
  ASSERT_FALSE(RandomDecision(Percent(50)));

  OverrideRandomDecisionForTesting(Percent(99));
  ASSERT_FALSE(RandomDecision(Percent(1)));

  OverrideRandomDecisionForTesting(Percent(99));
  ASSERT_FALSE(RandomDecision(Percent(0)));
}

TEST(RandomTest, OverrideToHalfPercent) {
  OverrideRandomDecisionForTesting(Percent(49));
  ASSERT_TRUE(RandomDecision(Percent(100)));

  OverrideRandomDecisionForTesting(Percent(49));
  ASSERT_TRUE(RandomDecision(Percent(99)));

  OverrideRandomDecisionForTesting(Percent(49));
  ASSERT_TRUE(RandomDecision(Percent(50)));

  OverrideRandomDecisionForTesting(Percent(49));
  ASSERT_FALSE(RandomDecision(Percent(1)));

  OverrideRandomDecisionForTesting(Percent(49));
  ASSERT_FALSE(RandomDecision(Percent(0)));
}

TEST(RandomTest, OverrideToAllPercent) {
  OverrideRandomDecisionForTesting(Percent(0));
  ASSERT_TRUE(RandomDecision(Percent(100)));

  OverrideRandomDecisionForTesting(Percent(0));
  ASSERT_TRUE(RandomDecision(Percent(99)));

  OverrideRandomDecisionForTesting(Percent(0));
  ASSERT_TRUE(RandomDecision(Percent(50)));

  OverrideRandomDecisionForTesting(Percent(0));
  ASSERT_TRUE(RandomDecision(Percent(1)));

  OverrideRandomDecisionForTesting(Percent(0));
  ASSERT_FALSE(RandomDecision(Percent(0)));
}

TEST(RandomTest, OverrideValue) {
  OverrideRandomValueForTesting(1337);
  ASSERT_EQ(RandomValue(0, 1000), 1337);

  OverrideRandomValueForTesting(1337);
  ASSERT_EQ(RandomValue(0, 1), 1337);
}

TEST(RandomTest, ChanceForTrueIsCorrect) {
  int kLarge = 100000000;
  int hits = 0;
  for (int i = 0; i < kLarge; ++i) {
    if (RandomDecision(Percent(50))) {
      ++hits;
    }
  }
  ASSERT_TRUE(std::abs(static_cast<double>(hits) / kLarge - 0.5) < 0.00001)
      << static_cast<double>(hits) / kLarge;
}