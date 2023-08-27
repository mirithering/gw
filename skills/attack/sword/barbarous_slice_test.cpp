#include "barbarous_slice.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "character/build.h"
#include "character/creature.h"
#include "weapon/sword.h"

namespace {
// TODO create a few files with helper classes and factory functions.
class NoOpStance : public Stance {
 public:
  NoOpStance() {}
  int BlockChance(Weapon::Type) override { return 0; }
  void AttackBlocked(Weapon::Type) override {}
};
}  // namespace

class BarbarousSliceTest : public ::testing::Test {
 public:
  void SetUp() override { character.target_ = &dummy; }

 protected:
  Creature character =
      ConstructCreature(Profession::Warrior, Sword(),
                        {{Attribute::Swordsmanship, 12}}, BarbarousSlice());
  Creature dummy = ConstructCreature(Profession::Warrior, Sword());
  BarbarousSlice* barbarous_slice =
      character.GetBuild().GetSkill<BarbarousSlice>(0);
  std::vector<Creature> empty_;
};

TEST_F(BarbarousSliceTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(barbarous_slice->CanActivate(character, empty_, empty_));
}

TEST_F(BarbarousSliceTest, NoActivationWithTooLittleAdrenaline) {
  barbarous_slice->AddAdrenaline(5 * 25);
  ASSERT_FALSE(barbarous_slice->CanActivate(character, empty_, empty_));
}

TEST_F(BarbarousSliceTest, ActivationWithEnoughAdrenaline) {
  barbarous_slice->AddAdrenaline(6 * 25);
  ASSERT_TRUE(barbarous_slice->CanActivate(character, empty_, empty_));
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsDamage) {
  constexpr int kExpectedSkillDamage = 25;

  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, empty_, empty_);

  // Override random base attack damage to 0. Then, only skill damage is
  // inflicted.
  OverrideRandomRollForTesting(0);
  // Put myself in a stance to avoid inflicting bleeding.
  character.SetStance(Effect<Stance>(10000, std::make_unique<NoOpStance>()));

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_EQ(dummy.GetLostHealth(), kExpectedSkillDamage);
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsBleedingIfNoStance) {
  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, empty_, empty_);

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_TRUE(dummy.HasCondition(Condition::Type::Bleeding));
}

TEST_F(BarbarousSliceTest, BarbarousSliceDoesNotInflictBleedingIfStance) {
  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, empty_, empty_);

  character.SetStance(Effect<Stance>(10000, std::make_unique<NoOpStance>()));

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_FALSE(dummy.HasCondition(Condition::Type::Bleeding));
}