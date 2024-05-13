#include "deep_wound.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "test/test.h"

class DeepWoundTest : public GwTest {};

TEST_F(DeepWoundTest, Test) {
  auto creature = AddAssassinTo(team());
  int previous_health = creature->GetMaxHealth();
  creature->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<DeepWound>()));
  ASSERT_TRUE(creature->HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(creature->GetMaxHealth(), previous_health * 0.8);
}