#include "bonettis_defense.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "bonettis_defense.h"
#include "character/character.h"

class BonettisDefenseTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto skill_ptr = std::make_unique<BonettisDefense>();
    bonettis_defense = skill_ptr.get();
    character.SetSkill(0, std::move(skill_ptr));
  }

 protected:
  Character character{Profession::Warrior};
  Skill* bonettis_defense;
};

TEST_F(BonettisDefenseTest, CannotUseWithoutAdrenaline) {
  EXPECT_FALSE(bonettis_defense->CanActivate(character));
}

TEST_F(BonettisDefenseTest, UnskilledDurationIsFive) {
  bonettis_defense->AddAdrenaline(8 * 25);
  ASSERT_TRUE(bonettis_defense->CanActivate(character));

  character.GetAction() = bonettis_defense->Activate(character, character);
  for (int i = 1; i <= 5000; ++i) {
    ASSERT_NE(character.GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(character.GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, SkilledDurationIsTen) {
  character.SetAttribute(Attribute::Tactics, 12);
  bonettis_defense->AddAdrenaline(8 * 25);
  ASSERT_TRUE(bonettis_defense->CanActivate(character));

  character.GetAction() = bonettis_defense->Activate(character, character);
  for (int i = 1; i <= 10000; ++i) {
    ASSERT_NE(character.GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(character.GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, BlockChanceDependsOnWeaponType) {
  BonettisDefenseStance stance(character);
  EXPECT_EQ(stance.BlockChance(Weapon::Type::Hammer), 75);
  EXPECT_EQ(stance.BlockChance(Weapon::Type::Staff), 0);
}

TEST_F(BonettisDefenseTest, BlockingMeeleGivesEnergy) {
  BonettisDefenseStance stance(character);

  character.UseEnergy(character.energy());
  EXPECT_EQ(character.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Bow);
  EXPECT_EQ(character.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Sword);
  EXPECT_EQ(character.energy(), 5);
}
