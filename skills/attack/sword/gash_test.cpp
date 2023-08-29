#include "gash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "base/random.h"
#include "character/creature.h"
#include "character/damage.h"
#include "conditions/bleeding.h"
#include "weapon/sword.h"

class GashTest : public ::testing::Test {
 public:
  void SetUp() override { character_.target_ = &dummy_; }

 protected:
  Creature character_ = ConstructCreature(
      Profession::Warrior, Sword(), {{Attribute::Swordsmanship, 12}}, Gash());
  Creature dummy_ = ConstructCreature(Profession::Warrior, Sword());
  Gash* gash_ = character_.GetBuild().GetSkill<Gash>(0);
  std::vector<Creature> empty_;
};

TEST_F(GashTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(gash_->CanActivate(character_, empty_, empty_));
}

TEST_F(GashTest, ActivationWithAdrenaline) {
  gash_->AddAdrenaline(6 * Strike);
  ASSERT_TRUE(gash_->CanActivate(character_, empty_, empty_));
}

TEST_F(GashTest, GashIsNormalAttackIfNotBleeding) {
  gash_->AddAdrenaline(6 * Strike);

  OverrideRandomRollForTesting(10);
  int expected_damage = WeaponStrikeDamage(character_, dummy_);

  OverrideRandomRollForTesting(10);
  character_.GetAction() = gash_->Activate(character_, empty_, empty_);
  while (character_.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_EQ(dummy_.GetLostHealth(), expected_damage);
  ASSERT_FALSE(dummy_.HasCondition(Condition::Type::DeepWound));
}

TEST_F(GashTest, GashHasAdditionalDamageIfBleeding) {
  constexpr int kExpectedSkillDamage = 17;
  constexpr int kExpectedBleedingDamage = 6;
  gash_->AddAdrenaline(6 * Strike);

  OverrideRandomRollForTesting(10);
  int expected_damage = WeaponStrikeDamage(character_, dummy_) +
                        kExpectedSkillDamage + kExpectedBleedingDamage;

  dummy_.AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>(dummy_)));
  ASSERT_TRUE(dummy_.HasCondition(Condition::Type::Bleeding));

  OverrideRandomRollForTesting(10);
  character_.GetAction() = gash_->Activate(character_, empty_, empty_);
  while (character_.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_TRUE(dummy_.HasCondition(Condition::Type::Bleeding));
  ASSERT_TRUE(dummy_.HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(dummy_.GetLostHealth(), expected_damage);
}

TEST_F(GashTest, GashIsInflictsDeepWoundIfBleeding) {
  gash_->AddAdrenaline(6 * Strike);

  dummy_.AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>(dummy_)));
  character_.GetAction() = gash_->Activate(character_, empty_, empty_);
  while (character_.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_TRUE(dummy_.HasCondition(Condition::Type::DeepWound));
}