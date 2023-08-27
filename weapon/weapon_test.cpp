#include "weapon.h"

#include <gtest/gtest.h>

TEST(WeaponTest, MeeleWeapons) {
  ASSERT_TRUE(IsMeele(Weapon::Type::Axe));
  ASSERT_TRUE(IsMeele(Weapon::Type::Sword));
  ASSERT_TRUE(IsMeele(Weapon::Type::Dagger));
  ASSERT_TRUE(IsMeele(Weapon::Type::Hammer));
  ASSERT_TRUE(IsMeele(Weapon::Type::Scythe));
  ASSERT_FALSE(IsMeele(Weapon::Type::Spear));
  ASSERT_FALSE(IsMeele(Weapon::Type::Bow));
  ASSERT_FALSE(IsMeele(Weapon::Type::Wand));
  ASSERT_FALSE(IsMeele(Weapon::Type::Staff));
  ASSERT_FALSE(IsMeele(Weapon::Type::Focus));
  ASSERT_FALSE(IsMeele(Weapon::Type::Shield));
}

TEST(WeaponTest, ProjectileWeapons) {
  ASSERT_FALSE(IsProjectile(Weapon::Type::Axe));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Sword));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Dagger));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Hammer));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Scythe));
  ASSERT_TRUE(IsProjectile(Weapon::Type::Spear));
  ASSERT_TRUE(IsProjectile(Weapon::Type::Bow));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Wand));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Staff));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Focus));
  ASSERT_FALSE(IsProjectile(Weapon::Type::Shield));
}