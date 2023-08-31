#include "pure_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "weapon/sword.h"

class PureStrikeTest : public ::testing::Test {
  void SetUp() override { character.target_ = &dummy; }

 protected:
  Creature character =
      ConstructCreature(Profession::Warrior, Sword(),
                        {{Attribute::Swordsmanship, 12}}, PureStrike());
  Creature dummy = ConstructCreature(Profession::Warrior, Sword());
  std::vector<Creature> kEmpty;
};

TEST_F(PureStrikeTest, PureStrikeCannotBeBlockedIfNotInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy.SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));
  character.GetAction() = character.GetBuild().GetSkill<Skill>(0)->Activate(
      character, kEmpty, kEmpty);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
  ASSERT_EQ(stance->attack_blocked_called_, 0);
  ASSERT_NE(dummy.GetLostHealth(), 0);
}

TEST_F(PureStrikeTest, PureStrikeCanBeBlockedIfInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy.SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));
  static_cast<BlockEverythingStance*>(character.SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));

  character.GetAction() = character.GetBuild().GetSkill<Skill>(0)->Activate(
      character, kEmpty, kEmpty);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
  ASSERT_EQ(stance->attack_blocked_called_, 1);
  ASSERT_EQ(dummy.GetLostHealth(), 0);
}