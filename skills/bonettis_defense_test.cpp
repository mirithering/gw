#include "bonettis_defense.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "bonettis_defense.h"
#include "character/creature.h"
#include "weapon/sword.h"

class BonettisDefenseTest : public ::testing::Test {
 protected:
  Creature character_ =
      ConstructCreature(Profession::Warrior, Sword(), {}, BonettisDefense());
  BonettisDefense* bonettis_defense_ =
      character_.GetBuild().GetSkill<BonettisDefense>(0);
  std::vector<Creature> kEmpty;
};

TEST_F(BonettisDefenseTest, CannotUseWithoutAdrenaline) {
  ASSERT_FALSE(bonettis_defense_->CanActivate(character_, {}, {}));
}

TEST_F(BonettisDefenseTest, UnskilledDurationIsFive) {
  bonettis_defense_->AddAdrenaline(8 * 25);
  ASSERT_TRUE(bonettis_defense_->CanActivate(character_, {}, {}));

  character_.GetAction() =
      bonettis_defense_->Activate(character_, kEmpty, kEmpty);
  for (int i = 1; i <= 5000; ++i) {
    ASSERT_NE(character_.GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(character_.GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, SkilledDurationIsTen) {
  character_.GetBuild().SetAttribute(Attribute::Tactics, 12);
  bonettis_defense_->AddAdrenaline(8 * 25);
  ASSERT_TRUE(bonettis_defense_->CanActivate(character_, {}, {}));

  character_.GetAction() =
      bonettis_defense_->Activate(character_, kEmpty, kEmpty);
  for (int i = 1; i <= 10000; ++i) {
    ASSERT_NE(character_.GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(character_.GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, BlockChanceDependsOnWeaponType) {
  BonettisDefenseStance stance(character_);
  ASSERT_EQ(stance.BlockChance(Weapon::Type::Hammer), 75);
  ASSERT_EQ(stance.BlockChance(Weapon::Type::Staff), 0);
}

TEST_F(BonettisDefenseTest, BlockingMeeleGivesEnergy) {
  BonettisDefenseStance stance(character_);

  character_.UseEnergy(character_.energy());
  ASSERT_EQ(character_.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Bow);
  ASSERT_EQ(character_.energy(), 0);

  stance.AttackBlocked(Weapon::Type::Sword);
  ASSERT_EQ(character_.energy(), 5);
}
