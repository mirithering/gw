#include "gash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "base/random.h"
#include "character/character.h"
#include "character/damage.h"
#include "conditions/bleeding.h"
#include "weapon/sword.h"

class GashTest : public ::testing::Test {
 public:
  void SetUp() override {
    gash = static_cast<Gash*>(character.SetSkill(0, std::make_unique<Gash>()));
    character.SetAttribute(Attribute::Swordsmanship, 12);
    character.GiveWeapon(std::make_unique<Sword>());
  }

 protected:
  Character character{Profession::Warrior};
  Character dummy{Profession::Warrior};
  Gash* gash;
};

TEST_F(GashTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(gash->CanActivate(character));
}

TEST_F(GashTest, ActivationWithAdrenaline) {
  gash->AddAdrenaline(6 * 25);
  ASSERT_TRUE(gash->CanActivate(character));
}

TEST_F(GashTest, GashIsNormalAttackIfNotBleeding) {
  gash->AddAdrenaline(6 * 25);

  OverrideRandomRollForTesting(10);
  int expected_damage = WeaponStrikeDamage(character, dummy);

  OverrideRandomRollForTesting(10);
  character.GetAction() = gash->Activate(character, dummy);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_EQ(dummy.health(), dummy.GetMaxHealth() - expected_damage);
  EXPECT_FALSE(dummy.HasCondition(Condition::Type::DeepWound));
}

TEST_F(GashTest, GashHasAdditionalDamageIfBleeding) {
  constexpr int kExpectedSkillDamage = 17;
  constexpr int kExpectedBleedingDamage = 6;
  constexpr int kExpectedDeepWoundDamage =
      0;  // TODO change once deep wound has an effect.
  gash->AddAdrenaline(6 * 25);

  OverrideRandomRollForTesting(10);
  int expected_damage = WeaponStrikeDamage(character, dummy) +
                        kExpectedSkillDamage + kExpectedBleedingDamage +
                        kExpectedDeepWoundDamage;

  dummy.AddCondition(Effect<Condition>(10000, std::make_unique<Bleeding>()));

  OverrideRandomRollForTesting(10);
  character.GetAction() = gash->Activate(character, dummy);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_EQ(dummy.health(), dummy.GetMaxHealth() - expected_damage);
}

TEST_F(GashTest, GashIsInflictsDeepWoundIfBleeding) {
  gash->AddAdrenaline(6 * 25);

  dummy.AddCondition(Effect<Condition>(10000, std::make_unique<Bleeding>()));
  character.GetAction() = gash->Activate(character, dummy);
  while (character.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_TRUE(dummy.HasCondition(Condition::Type::DeepWound));
}