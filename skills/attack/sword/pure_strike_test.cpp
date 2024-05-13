#include "pure_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect_deprecated.h"
#include "test/test.h"
#include "weapon/sword.h"

class PureStrikeTest : public GwTest {
  void SetUp() override {
    creature_ = AddWarriorTo(team());
    creature_->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    pure_strike_ =
        creature_->GetBuild().AddSkill(std::make_unique<PureStrike>());

    dummy_ = AddWarriorTo(enemies());

    creature_->target_ = dummy_;
  }

 protected:
  Creature* creature_;
  PureStrike* pure_strike_;
  Creature* dummy_;
};

TEST_F(PureStrikeTest, PureStrikeCannotBeBlockedIfNotInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(
      dummy_->SetStance(EffectDeprecated<Stance>(
          Eternity, std::make_unique<BlockEverythingStance>())));
  creature_->UseSkill(pure_strike_, world());
  AwaitIdle(creature_);
  ASSERT_EQ(stance->attack_blocked_called_, 0);
  ASSERT_NE(dummy_->GetLostHealth(), 0);
}

TEST_F(PureStrikeTest, PureStrikeCanBeBlockedIfInStance) {
  auto* stance = static_cast<BlockEverythingStance*>(
      dummy_->SetStance(EffectDeprecated<Stance>(
          Eternity, std::make_unique<BlockEverythingStance>())));
  static_cast<BlockEverythingStance*>(
      creature_->SetStance(EffectDeprecated<Stance>(
          Eternity, std::make_unique<BlockEverythingStance>())));
  creature_->UseSkill(pure_strike_, world());
  AwaitIdle(creature_);
  ASSERT_EQ(stance->attack_blocked_called_, 1);
  ASSERT_EQ(dummy_->GetLostHealth(), 0);
}