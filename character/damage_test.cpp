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
  auto attacker =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  attacker->GetBuild().SetArmor(std::make_unique<Armor>(0));

  auto defender =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  defender->GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kDamage = 10;

  OverrideRandomValueForTesting(kDamage);

  EXPECT_EQ(WeaponStrikeDamage(*attacker, *defender),
            static_cast<int>(kDamage * kExpectedReqNotMetModifier));
}

TEST(DamageTest, DamageWithoutArmorIsBaseDamageModifiedByAttribute) {
  const int kAttribute = 9;

  auto attacker =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(),
                        {{Attribute::Swordsmanship, kAttribute}});
  attacker->GetBuild().SetArmor(std::make_unique<Armor>(0));

  auto defender =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  defender->GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kDamage = 20;

  OverrideRandomValueForTesting(kDamage);

  double expected_factor = std::pow(2.0, 5 * kAttribute / 40.0);

  EXPECT_EQ(WeaponStrikeDamage(*attacker, *defender),
            static_cast<int>(kDamage * expected_factor));
}

TEST(DamageTest, DamageWithoutAttributeIsBaseDamageModifiedByArmor) {
  auto attacker =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  attacker->GetBuild().SetArmor(std::make_unique<Armor>(0));

  const int kArmor = 10;

  auto defender =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  defender->GetBuild().SetArmor(std::make_unique<Armor>(kArmor));

  const int kDamage = 16;

  OverrideRandomValueForTesting(kDamage);

  double expected_factor = std::pow(2.0, (-kArmor) / 40.0);

  EXPECT_EQ(
      WeaponStrikeDamage(*attacker, *defender),
      static_cast<int>(kDamage * kExpectedReqNotMetModifier * expected_factor));
}

TEST(DamageTest, DamageWithAttributeAndArmorIsExpectedFormula) {
  const int kAttribute = 9;
  const int kArmor = 10;
  const int kDamage = 25;
  double expected_factor = std::pow(2.0, (5 * kAttribute - kArmor) / 40.0);

  OverrideRandomValueForTesting(kDamage);

  auto attacker =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(),
                        {{Attribute::Swordsmanship, kAttribute}});
  attacker->GetBuild().SetArmor(std::make_unique<Armor>(0));

  auto defender =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>(), {});
  defender->GetBuild().SetArmor(std::make_unique<Armor>(kArmor));

  EXPECT_EQ(WeaponStrikeDamage(*attacker, *defender),
            static_cast<int>(kDamage * expected_factor));
}