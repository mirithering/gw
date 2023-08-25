#include "sword.h"

#include <gtest/gtest.h>

TEST(SwordTest, AttackSpeedIsAsExpected) {
  constexpr int kExpectedAttackSpeed = 1330;

  Sword sword;
  EXPECT_EQ(sword.AttackSpeed(), kExpectedAttackSpeed);
}

TEST(SwordTest, DamageTypeIsSlashing) {
  Sword sword;
  EXPECT_EQ(sword.GetDamageType(), DamageType::Slashing);
}

TEST(SwordTest, AttributeIsSwordsmanship) {
  Sword sword;
  EXPECT_EQ(sword.GetAttribute(), Attribute::Swordsmanship);
}

TEST(SwordTest, TypeIsSword) {
  Sword sword;
  EXPECT_EQ(sword.GetType(), Weapon::Type::Sword);
}

TEST(SwordTest, DefaultDamageIsAsExpected) {
  constexpr int kExpectedDefaultMinDamage = 15;
  constexpr int kExpectedDefaultMaxDamage = 22;

  Sword sword;
  EXPECT_EQ(sword.MinDamage(), kExpectedDefaultMinDamage);
  EXPECT_EQ(sword.MaxDamage(), kExpectedDefaultMaxDamage);
}

TEST(SwordTest, DamageIsSetWithConstructor) {
  constexpr int kMinDamage = 4;
  constexpr int kMaxDamage = 45;

  Sword sword(kMinDamage, kMaxDamage);
  EXPECT_EQ(sword.MinDamage(), kMinDamage);
  EXPECT_EQ(sword.MaxDamage(), kMaxDamage);
}

TEST(SwordTest, DefaultRequirementIsAsExpected) {
  constexpr int kDefaultReq = 9;

  Sword sword;
  EXPECT_EQ(sword.Requirement(), kDefaultReq);
}