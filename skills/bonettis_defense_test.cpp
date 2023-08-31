#include "bonettis_defense.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
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
  bonettis_defense_->AddAdrenaline(8 * Strike);
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
  bonettis_defense_->AddAdrenaline(8 * Strike);
  ASSERT_TRUE(bonettis_defense_->CanActivate(character_, {}, {}));

  character_.GetAction() =
      bonettis_defense_->Activate(character_, kEmpty, kEmpty);
  for (int i = 1; i <= 10000; ++i) {
    ASSERT_NE(character_.GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(character_.GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, BlockChanceIs75ForMeele) {
  character_.SetStance(
      Effect<Stance>(20 * Second, std::make_unique<BonettisDefenseStance>()));

  Creature attacker = ConstructCreature(Profession::Warrior, Sword());

  OverrideRandomDecisionForTesting(Percent(74));
  ASSERT_FALSE(attacker.WeaponAttack(character_));
  OverrideRandomDecisionForTesting(Percent(75));
  ASSERT_TRUE(attacker.WeaponAttack(character_));
}

// TODO write a test for projectile and one for staff.

TEST_F(BonettisDefenseTest, BlockingMeeleGivesEnergy) {
  character_.SetStance(
      Effect<Stance>(20 * Second, std::make_unique<BonettisDefenseStance>()));

  Creature attacker = ConstructCreature(Profession::Warrior, Sword());

  character_.UseEnergy(character_.energy());
  ASSERT_EQ(character_.energy(), 0);
  OverrideRandomDecisionForTesting(Percent(74));
  ASSERT_FALSE(attacker.WeaponAttack(character_));
  ASSERT_EQ(character_.energy(), 5);
}
