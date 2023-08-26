#include "pure_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "weapon/sword.h"

namespace {
// TODO create a few files with helper classes and factory functions.
class BlockEverythingStance : public Stance {
 public:
  BlockEverythingStance() {}
  int BlockChance(Weapon::Type) override { return 100; }
  void AttackBlocked(Weapon::Type) override { ++attack_blocked_called_; }
  int attack_blocked_called_ = 0;
};
}  // namespace

class PureStrikeTest : public ::testing::Test {
 public:
  void SetUp() override {
    character.SetSkill(0, std::make_unique<PureStrike>());
    character.SetAttribute(Attribute::Swordsmanship, 12);
    character.GiveWeapon(std::make_unique<Sword>());
  }

 protected:
  Character character{Profession::Warrior};
  Character dummy{Profession::Warrior};
};

TEST_F(PureStrikeTest, PureStrikeCannotBeBlockedIfNotInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy.SetStance(
      Effect<Stance>(INT_MAX, std::make_unique<BlockEverythingStance>())));
  character.GetAction() = character.GetSkill(0)->Activate(character, dummy);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
  ASSERT_EQ(stance->attack_blocked_called_, 0);
  ASSERT_NE(dummy.health(), dummy.GetMaxHealth());
}

TEST_F(PureStrikeTest, PureStrikeCanBeBlockedIfInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy.SetStance(
      Effect<Stance>(INT_MAX, std::make_unique<BlockEverythingStance>())));
  static_cast<BlockEverythingStance*>(character.SetStance(
      Effect<Stance>(INT_MAX, std::make_unique<BlockEverythingStance>())));

  character.GetAction() = character.GetSkill(0)->Activate(character, dummy);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
  ASSERT_EQ(stance->attack_blocked_called_, 1);
  ASSERT_EQ(dummy.health(), dummy.GetMaxHealth());
}