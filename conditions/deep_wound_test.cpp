#include "deep_wound.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "test/test.h"

class DeepWoundTest : public GwTest {};

TEST_F(DeepWoundTest, Test) {
  auto character = AddAssassinTo(team());
  int previous_health = character->GetMaxHealth();
  character->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<DeepWound>()));
  ASSERT_TRUE(character->HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(character->GetMaxHealth(), previous_health * 0.8);
}