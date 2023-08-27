#include "dagger.h"

#include <gtest/gtest.h>

TEST(DaggerTest, AttackSpeedIsAsExpected) {
  constexpr int kExpectedAttackSpeed = 1330;

  Dagger dagger;
  ASSERT_EQ(dagger.AttackSpeed(), kExpectedAttackSpeed);
}

TEST(DaggerTest, DamageTypeIsPiercing) {
  Dagger dagger;
  ASSERT_EQ(dagger.GetDamageType(), DamageType::Piercing);
}

TEST(DaggerTest, AttributeIsDaggerMastery) {
  Dagger dagger;
  ASSERT_EQ(dagger.GetAttribute(), Attribute::DaggerMastery);
}

TEST(DaggerTest, TypeIsDagger) {
  Dagger dagger;
  ASSERT_EQ(dagger.GetType(), Weapon::Type::Dagger);
}

TEST(DaggerTest, DefaultDamageIsAsExpected) {
  constexpr int kExpectedDefaultMinDamage = 7;
  constexpr int kExpectedDefaultMaxDamage = 17;

  Dagger dagger;
  ASSERT_EQ(dagger.MinDamage(), kExpectedDefaultMinDamage);
  ASSERT_EQ(dagger.MaxDamage(), kExpectedDefaultMaxDamage);
}

TEST(DaggerTest, DamageIsSetWithConstructor) {
  constexpr int kMinDamage = 5;
  constexpr int kMaxDamage = 89;

  Dagger dagger(kMinDamage, kMaxDamage);
  ASSERT_EQ(dagger.MinDamage(), kMinDamage);
  ASSERT_EQ(dagger.MaxDamage(), kMaxDamage);
}

TEST(DaggerTest, DefaultRequirementIsAsExpected) {
  constexpr int kDefaultReq = 9;

  Dagger dagger;
  ASSERT_EQ(dagger.Requirement(), kDefaultReq);
}