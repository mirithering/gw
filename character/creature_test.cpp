#include "creature.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/profession.h"
#include "conditions/bleeding.h"
#include "conditions/deep_wound.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/bonettis_defense.h"
#include "test/test.h"
#include "weapon/sword.h"

class CreatureTest : public GwTest {
 public:
  void SetUp() override {
    creature_ = AddWarriorTo(team());
    bonettis_defense_ =
        creature_->GetBuild().AddSkill(std::make_unique<BonettisDefense>());
    barbarous_slice_ =
        creature_->GetBuild().AddSkill(std::make_unique<BarbarousSlice>());

    dummy_ = AddWarriorTo(enemies());
  }

 protected:
  BonettisDefense* bonettis_defense_;
  BarbarousSlice* barbarous_slice_;
  Creature* creature_;
  Creature* dummy_;
};

TEST_F(CreatureTest, WarriorRecharges2EnergyEvery3000Ticks) {
  creature_->UseEnergy(5);
  int energy = creature_->energy();
  for (int i = 0; Time(i) < 3 * Second; ++i) Tick();
  ASSERT_EQ(creature_->energy(), energy + 2);
}

TEST_F(CreatureTest, StanceBlocksAttacks) {
  BlockEverythingStance* stance =
      static_cast<BlockEverythingStance*>(creature_->SetStance(
          Effect<Stance>(Second, std::make_unique<BlockEverythingStance>())));

  ASSERT_FALSE(creature_->ReceiveWeaponDamage(100, Weapon::Type::Scythe));
  ASSERT_EQ(creature_->GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 1);

  ASSERT_FALSE(creature_->ReceiveWeaponDamage(200, Weapon::Type::Staff));
  ASSERT_EQ(creature_->GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 2);
}

TEST_F(CreatureTest, StartWithNoAdrenaline) {
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CreatureTest, OneHitBuildsOneStrikeOfAdrenalineOnAllSkills) {
  creature_->WeaponAttack(*dummy_);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), Strike);
}

TEST_F(CreatureTest, GettingHitBuildsPercentageOfAdrenalineOnAllSkills) {
  creature_->ReceiveWeaponDamage(creature_->GetMaxHealth() / 2,
                                 Weapon::Type::Axe);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 2 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 2 * Strike);
}

TEST_F(CreatureTest, DeathTakesAllAdrenaline) {
  bonettis_defense_->AddAdrenaline(4 * Strike);
  barbarous_slice_->AddAdrenaline(2 * Strike);

  creature_->ReceiveWeaponDamage(creature_->GetMaxHealth(), Weapon::Type::Axe);
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CreatureTest, UsingSkillRemovesOneAdrenalineStrikeFromOthers) {
  bonettis_defense_->AddAdrenaline(10 * Strike);
  barbarous_slice_->AddAdrenaline(10 * Strike);

  World empty;

  bonettis_defense_->Activate(*creature_, empty);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 9 * Strike);
}

TEST_F(CreatureTest, UsingSkillRemovesAllAdrenalingFromItself) {
  barbarous_slice_->AddAdrenaline(10 * Strike);

  World empty;

  barbarous_slice_->Activate(*creature_, empty);

  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CreatureTest, AddConditionAddsCondition) {
  creature_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
}

TEST_F(CreatureTest, AddTwoConditionsAddsTwoCondition) {
  creature_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
  creature_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<DeepWound>()));
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::DeepWound));
}

TEST_F(CreatureTest, SecondConditionOverridesIfLonger) {
  creature_->AddCondition(
      Effect<Condition>(9 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
  auto bleeding_ = creature_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_NE(bleeding_, nullptr);
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
}

TEST_F(CreatureTest, SecondConditionDoesNotOverrideIfShorter) {
  creature_->AddCondition(
      Effect<Condition>(9 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
  auto bleeding_ = creature_->AddCondition(
      Effect<Condition>(8 * Second, std::make_unique<Bleeding>()));
  ASSERT_EQ(bleeding_, nullptr);
  ASSERT_TRUE(creature_->HasCondition(Condition::Type::Bleeding));
}
