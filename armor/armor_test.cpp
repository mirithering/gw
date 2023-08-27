#include "armor.h"

#include <gtest/gtest.h>

#include "base/profession.h"

TEST(ArmorTest, ArmorWarriorHasExpectedArmor) {
  constexpr int kExpectedArmor = 80;

  Armor armor(Profession::Warrior);
  ASSERT_EQ(armor.Rating(), kExpectedArmor);
}

TEST(ArmorTest, ArmorAssassinHasExpectedArmor) {
  constexpr int kExpectedArmor = 70;

  Armor armor(Profession::Assassin);
  ASSERT_EQ(armor.Rating(), kExpectedArmor);
}