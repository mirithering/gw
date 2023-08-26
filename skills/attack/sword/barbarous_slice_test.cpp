#include "barbarous_slice.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "character/character.h"
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
  void SetUp() override {
    barbarous_slice = static_cast<BarbarousSlice*>(
        character.SetSkill(0, std::make_unique<BarbarousSlice>()));
    character.SetAttribute(Attribute::Swordsmanship, 12);
    character.GiveWeapon(std::make_unique<Sword>());
  }

 protected:
  Character character{Profession::Warrior};
  Character dummy{Profession::Warrior};
  BarbarousSlice* barbarous_slice;
};

TEST_F(BarbarousSliceTest, NoActivationWithoutAdrenaline) {
  EXPECT_FALSE(barbarous_slice->CanActivate(character));
}

TEST_F(BarbarousSliceTest, NoActivationWithTooLittleAdrenaline) {
  barbarous_slice->AddAdrenaline(5 * 25);
  EXPECT_FALSE(barbarous_slice->CanActivate(character));
}

TEST_F(BarbarousSliceTest, ActivationWithEnoughAdrenaline) {
  barbarous_slice->AddAdrenaline(6 * 25);
  EXPECT_TRUE(barbarous_slice->CanActivate(character));
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsDamage) {
  constexpr int kExpectedSkillDamage = 25;

  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, dummy);

  // Override random base attack damage to 0. Then, only skill damage is
  // inflicted.
  OverrideRandomRollForTesting(0);
  // Put myself in a stance to avoid inflicting bleeding.
  character.SetStance(Effect<Stance>(10000, std::make_unique<NoOpStance>()));

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_EQ(dummy.GetLostHealth(), kExpectedSkillDamage);
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsBleedingIfNoStance) {
  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, dummy);

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_TRUE(dummy.HasCondition(Condition::Type::Bleeding));
}

TEST_F(BarbarousSliceTest, BarbarousSliceDoesNotInflictBleedingIfStance) {
  barbarous_slice->AddAdrenaline(6 * 25);
  character.GetAction() = barbarous_slice->Activate(character, dummy);

  character.SetStance(Effect<Stance>(10000, std::make_unique<NoOpStance>()));

  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_FALSE(dummy.HasCondition(Condition::Type::Bleeding));
}