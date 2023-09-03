#include "pure_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "test/test.h"
#include "weapon/sword.h"

class PureStrikeTest : public GwTest {
  void SetUp() override {
    character_ = AddWarriorTo(team());
    character_->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    pure_strike_ =
        character_->GetBuild().AddSkill(std::make_unique<PureStrike>());

    dummy_ = AddWarriorTo(enemies());

    character_->target_ = dummy_;
  }

 protected:
  Creature* character_;
  PureStrike* pure_strike_;
  Creature* dummy_;
};

TEST_F(PureStrikeTest, PureStrikeCannotBeBlockedIfNotInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy_->SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));
  character_->GetAction() =
      pure_strike_->Activate(*character_, team(), enemies());
  TickUntilIdle(character_);
  ASSERT_EQ(stance->attack_blocked_called_, 0);
  ASSERT_NE(dummy_->GetLostHealth(), 0);
}

TEST_F(PureStrikeTest, PureStrikeCanBeBlockedIfInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(dummy_->SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));
  static_cast<BlockEverythingStance*>(character_->SetStance(
      Effect<Stance>(Eternity, std::make_unique<BlockEverythingStance>())));

  character_->GetAction() =
      pure_strike_->Activate(*character_, team(), enemies());
  TickUntilIdle(character_);
  ASSERT_EQ(stance->attack_blocked_called_, 1);
  ASSERT_EQ(dummy_->GetLostHealth(), 0);
}