#include "bonettis_defense.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

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
  EXPECT_FALSE(bonettis_defense->CanUse(character));
}

TEST_F(BonettisDefenseTest, UnskilledDurationIsFive) {
  bonettis_defense->AddAdrenaline(bonettis_defense->AdrenalineCost());
  EXPECT_TRUE(bonettis_defense->CanUse(character));

  character.GetAction() = bonettis_defense->Use(character, character);
  for (int i = 1; i < 5000; ++i) {
    EXPECT_NE(character.GetStance(), nullptr);
    EXPECT_EQ(character.GetStance()->Tick(), Stance::State::Continue);
  }
  EXPECT_EQ(character.GetStance()->Tick(), Stance::State::End);
}

TEST_F(BonettisDefenseTest, SkilledDurationIsTen) {
  character.SetAttribute(Attribute::Tactics, 12);
  bonettis_defense->AddAdrenaline(bonettis_defense->AdrenalineCost());
  EXPECT_TRUE(bonettis_defense->CanUse(character));

  character.GetAction() = bonettis_defense->Use(character, character);
  for (int i = 1; i < 10000; ++i) {
    EXPECT_NE(character.GetStance(), nullptr);
    EXPECT_EQ(character.GetStance()->Tick(), Stance::State::Continue);
  }
  EXPECT_EQ(character.GetStance()->Tick(), Stance::State::End);
}

TEST_F(BonettisDefenseTest, BlockChanceDependsOnWeaponType) {
  BonettisDefenseStance stance(10, character);
  EXPECT_EQ(stance.BlockChance(Weapon::Type::Hammer), 75);
  EXPECT_EQ(stance.BlockChance(Weapon::Type::Staff), 0);
}

TEST_F(BonettisDefenseTest, BlockingMeeleGivesEnergy) {
  BonettisDefenseStance stance(10, character);

  character.UseEnergy(character.energy());
  EXPECT_EQ(character.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Bow);
  EXPECT_EQ(character.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Sword);
  EXPECT_EQ(character.energy(), 5);
}
