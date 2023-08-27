#include "sword.h"

#include <gtest/gtest.h>

TEST(SwordTest, AttackSpeedIsAsExpected) {
  constexpr int kExpectedAttackSpeed = 1330;

  Sword sword;
  ASSERT_EQ(sword.AttackSpeed(), kExpectedAttackSpeed);
}

TEST(SwordTest, DamageTypeIsSlashing) {
  Sword sword;
  ASSERT_EQ(sword.GetDamageType(), DamageType::Slashing);
}

TEST(SwordTest, AttributeIsSwordsmanship) {
  Sword sword;
  ASSERT_EQ(sword.GetAttribute(), Attribute::Swordsmanship);
}

TEST(SwordTest, TypeIsSword) {
  Sword sword;
  ASSERT_EQ(sword.GetType(), Weapon::Type::Sword);
}

TEST(SwordTest, DefaultDamageIsAsExpected) {
  constexpr int kExpectedDefaultMinDamage = 15;
  constexpr int kExpectedDefaultMaxDamage = 22;

  Sword sword;
  ASSERT_EQ(sword.MinDamage(), kExpectedDefaultMinDamage);
  ASSERT_EQ(sword.MaxDamage(), kExpectedDefaultMaxDamage);
}

TEST(SwordTest, DamageIsSetWithConstructor) {
  constexpr int kMinDamage = 4;
  constexpr int kMaxDamage = 45;

  Sword sword(kMinDamage, kMaxDamage);
  ASSERT_EQ(sword.MinDamage(), kMinDamage);
  ASSERT_EQ(sword.MaxDamage(), kMaxDamage);
}

TEST(SwordTest, DefaultRequirementIsAsExpected) {
  constexpr int kDefaultReq = 9;

  Sword sword;
  ASSERT_EQ(sword.Requirement(), kDefaultReq);
}