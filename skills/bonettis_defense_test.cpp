#include "bonettis_defense.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
#include "bonettis_defense.h"
#include "entities/creature.h"
#include "test/test.h"
#include "weapon/sword.h"

class BonettisDefenseTest : public GwTest {
 public:
  void SetUp() override {
    warrior_ = AddWarriorTo(team());
    bonettis_defense_ =
        warrior_->GetBuild().AddSkill(std::make_unique<BonettisDefense>());

    enemy_ = AddWarriorTo(enemies());
  }

 protected:
  Creature* warrior_;
  BonettisDefense* bonettis_defense_;

  Creature* enemy_;
};

TEST_F(BonettisDefenseTest, CannotUseWithoutAdrenaline) {
  ASSERT_FALSE(bonettis_defense_->CanActivate(*warrior_, world()));
}

TEST_F(BonettisDefenseTest, UnskilledDurationIsFive) {
  bonettis_defense_->AddAdrenaline(8 * Strike);
  ASSERT_TRUE(bonettis_defense_->CanActivate(*warrior_, world()));

  warrior_->UseSkill(bonettis_defense_, world());
  for (int i = 1; i <= 5000; ++i) {
    ASSERT_NE(warrior_->GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(warrior_->GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, SkilledDurationIsTen) {
  warrior_->GetBuild().SetAttribute(Attribute::Tactics, 12);
  bonettis_defense_->AddAdrenaline(8 * Strike);
  ASSERT_TRUE(bonettis_defense_->CanActivate(*warrior_, world()));

  warrior_->UseSkill(bonettis_defense_, world());
  for (int i = 1; Time(i) <= 10 * Second; ++i) {
    ASSERT_NE(warrior_->GetStance(), nullptr);
    Tick();
  }
  ASSERT_EQ(warrior_->GetStance(), nullptr);
}

TEST_F(BonettisDefenseTest, BlockChanceIs75ForMeele) {
  warrior_->SetStance(
      Effect<Stance>(20 * Second, std::make_unique<BonettisDefenseStance>()));

  OverrideRandomDecisionForTesting(Percent(74));
  ASSERT_FALSE(enemy_->WeaponAttack(*warrior_));
  OverrideRandomDecisionForTesting(Percent(75));
  ASSERT_TRUE(enemy_->WeaponAttack(*warrior_));
}

// TODO write a test for projectile and one for staff.

TEST_F(BonettisDefenseTest, BlockingMeeleGivesEnergy) {
  warrior_->SetStance(
      Effect<Stance>(20 * Second, std::make_unique<BonettisDefenseStance>()));

  warrior_->UseEnergy(warrior_->energy());
  ASSERT_EQ(warrior_->energy(), 0);
  OverrideRandomDecisionForTesting(Percent(74));
  ASSERT_FALSE(enemy_->WeaponAttack(*warrior_));
  ASSERT_EQ(warrior_->energy(), 5);
}
