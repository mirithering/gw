#include "armor.h"

#include <gtest/gtest.h>

#include "base/profession.h"

TEST(ArmorTest, ArmorWarriorHasExpectedArmor) {
  constexpr int kExpectedArmor = 80;

  Armor armor(Profession::Warrior);
  EXPECT_EQ(armor.Rating(), kExpectedArmor);
}

TEST(ArmorTest, ArmorAssassinHasExpectedArmor) {
  constexpr int kExpectedArmor = 70;

  Armor armor(Profession::Assassin);
  EXPECT_EQ(armor.Rating(), kExpectedArmor);
}