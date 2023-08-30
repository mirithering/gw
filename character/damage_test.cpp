#include "damage.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "creature.h"
#include "weapon/sword.h"

namespace {
double kExpectedReqNotMetModifier = 1.0 / 3.0;
}

TEST(DamageTest, DamageWithoutArmorOrAttributeIsBaseDamage) {
  Creature attacker = ConstructCreature(Profession::Warrior, Sword(), {});
  attacker.GetBuild().SetArmor(std::make_unique<Armor>(0));

  Creature defender = ConstructCreature(Profession::Warrior, Sword(), {});
  defender.GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kDamage = 10;

  OverrideRandomRollForTesting(kDamage);

  EXPECT_EQ(WeaponStrikeDamage(attacker, defender),
            static_cast<int>(kDamage * kExpectedReqNotMetModifier));
}

TEST(DamageTest, DamageWithoutArmorIsBaseDamageModifiedByAttribute) {
  const int kAttribute = 9;

  Creature attacker = ConstructCreature(
      Profession::Warrior, Sword(), {{Attribute::Swordsmanship, kAttribute}});
  attacker.GetBuild().SetArmor(std::make_unique<Armor>(0));

  Creature defender = ConstructCreature(Profession::Warrior, Sword(), {});
  defender.GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kDamage = 20;

  OverrideRandomRollForTesting(kDamage);

  double expected_factor = std::pow(2.0, 5 * kAttribute / 40.0);

  EXPECT_EQ(WeaponStrikeDamage(attacker, defender),
            static_cast<int>(kDamage * expected_factor));
}

TEST(DamageTest, DamageWithoutAttributeIsBaseDamageModifiedByArmor) {
  Creature attacker = ConstructCreature(Profession::Warrior, Sword(), {});
  attacker.GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kArmor = 10;

  Creature defender = ConstructCreature(Profession::Warrior, Sword(), {});
  defender.GetBuild().SetArmor(std::make_unique<Armor>(kArmor));

  const int kDamage = 16;

  OverrideRandomRollForTesting(kDamage);

  double expected_factor = std::pow(2.0, (-kArmor) / 40.0);

  EXPECT_EQ(
      WeaponStrikeDamage(attacker, defender),
      static_cast<int>(kDamage * kExpectedReqNotMetModifier * expected_factor));
}

TEST(DamageTest, DamageWithAttributeAndArmorIsExpectedFormula) {
  const int kAttribute = 9;
  const int kArmor = 10;
  const int kDamage = 25;
  double expected_factor = std::pow(2.0, (5 * kAttribute - kArmor) / 40.0);

  OverrideRandomRollForTesting(kDamage);

  Creature attacker = ConstructCreature(
      Profession::Warrior, Sword(), {{Attribute::Swordsmanship, kAttribute}});
  attacker.GetBuild().SetArmor(std::make_unique<Armor>(0));

  Creature defender = ConstructCreature(Profession::Warrior, Sword(), {});
  defender.GetBuild().SetArmor(std::make_unique<Armor>(kArmor));

  EXPECT_EQ(WeaponStrikeDamage(attacker, defender),
            static_cast<int>(kDamage * expected_factor));
}