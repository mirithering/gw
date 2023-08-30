#include "deep_wound.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "weapon/sword.h"

TEST(DeepWoundTest, Test) {
  Creature creature = ConstructCreature(Profession::Assassin, Sword());
  int previous_health = creature.GetMaxHealth();
  creature.AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<DeepWound>()));
  ASSERT_TRUE(creature.HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(creature.GetMaxHealth(), previous_health * 0.8);
}