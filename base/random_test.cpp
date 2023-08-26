#include "random.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

TEST(RandomTest, OverrideToNoChance) {
  OverrideRandomRollForTesting(99);
  EXPECT_TRUE(RandomDecision(Chance(100)));

  OverrideRandomRollForTesting(99);
  EXPECT_FALSE(RandomDecision(Chance(99)));

  OverrideRandomRollForTesting(99);
  EXPECT_FALSE(RandomDecision(Chance(50)));

  OverrideRandomRollForTesting(99);
  EXPECT_FALSE(RandomDecision(Chance(1)));

  OverrideRandomRollForTesting(99);
  EXPECT_FALSE(RandomDecision(Chance(0)));
}

TEST(RandomTest, OverrideToHalfChance) {
  OverrideRandomRollForTesting(49);
  EXPECT_TRUE(RandomDecision(Chance(100)));

  OverrideRandomRollForTesting(49);
  EXPECT_TRUE(RandomDecision(Chance(99)));

  OverrideRandomRollForTesting(49);
  EXPECT_TRUE(RandomDecision(Chance(50)));

  OverrideRandomRollForTesting(49);
  EXPECT_FALSE(RandomDecision(Chance(1)));

  OverrideRandomRollForTesting(49);
  EXPECT_FALSE(RandomDecision(Chance(0)));
}

TEST(RandomTest, OverrideToAllChance) {
  OverrideRandomRollForTesting(0);
  EXPECT_TRUE(RandomDecision(Chance(100)));

  OverrideRandomRollForTesting(0);
  EXPECT_TRUE(RandomDecision(Chance(99)));

  OverrideRandomRollForTesting(0);
  EXPECT_TRUE(RandomDecision(Chance(50)));

  OverrideRandomRollForTesting(0);
  EXPECT_TRUE(RandomDecision(Chance(1)));

  OverrideRandomRollForTesting(0);
  EXPECT_FALSE(RandomDecision(Chance(0)));
}

TEST(RandomTest, OverrideValue) {
  OverrideRandomRollForTesting(1337);
  EXPECT_EQ(RandomValue(0, 1000), 1337);

  OverrideRandomRollForTesting(1337);
  EXPECT_EQ(RandomValue(0, 1), 1337);
}