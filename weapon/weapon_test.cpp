#include "weapon.h"

#include <gtest/gtest.h>

TEST(WeaponTest, MeeleWeapons) {
  EXPECT_TRUE(IsMeele(Weapon::Type::Axe));
  EXPECT_TRUE(IsMeele(Weapon::Type::Sword));
  EXPECT_TRUE(IsMeele(Weapon::Type::Dagger));
  EXPECT_TRUE(IsMeele(Weapon::Type::Hammer));
  EXPECT_TRUE(IsMeele(Weapon::Type::Scythe));
  EXPECT_FALSE(IsMeele(Weapon::Type::Spear));
  EXPECT_FALSE(IsMeele(Weapon::Type::Bow));
  EXPECT_FALSE(IsMeele(Weapon::Type::Wand));
  EXPECT_FALSE(IsMeele(Weapon::Type::Staff));
  EXPECT_FALSE(IsMeele(Weapon::Type::Focus));
  EXPECT_FALSE(IsMeele(Weapon::Type::Shield));
}

TEST(WeaponTest, ProjectileWeapons) {
  EXPECT_FALSE(IsProjectile(Weapon::Type::Axe));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Sword));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Dagger));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Hammer));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Scythe));
  EXPECT_TRUE(IsProjectile(Weapon::Type::Spear));
  EXPECT_TRUE(IsProjectile(Weapon::Type::Bow));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Wand));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Staff));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Focus));
  EXPECT_FALSE(IsProjectile(Weapon::Type::Shield));
}