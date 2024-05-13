#include "character.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/profession.h"
#include "conditions/bleeding.h"
#include "conditions/deep_wound.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/attack/sword/pure_strike.h"
#include "skills/bonettis_defense.h"
#include "test/test.h"
#include "weapon/sword.h"

class CharacterTest : public GwTest {
 public:
  void SetUp() override {
    character_ = AddWarriorTo(team());
    bonettis_defense_ =
        character_->GetBuild().AddSkill(std::make_unique<BonettisDefense>());
    barbarous_slice_ =
        character_->GetBuild().AddSkill(std::make_unique<BarbarousSlice>());
    pure_strike_ =
        character_->GetBuild().AddSkill(std::make_unique<PureStrike>());

    dummy_ = AddWarriorTo(enemies());
  }

 protected:
  BonettisDefense* bonettis_defense_;
  BarbarousSlice* barbarous_slice_;
  PureStrike* pure_strike_;

  Character* character_;
  Character* dummy_;
};

TEST_F(CharacterTest, WarriorRecharges2EnergyEvery3000Ticks) {
  character_->UseEnergy(5);
  int energy = character_->energy();
  for (int i = 0; Time(i) < 3 * Second; ++i) Tick();
  ASSERT_EQ(character_->energy(), energy + 2);
}

TEST_F(CharacterTest, StanceBlocksAttacks) {
  BlockEverythingStance* stance =
      static_cast<BlockEverythingStance*>(character_->SetStance(
          Effect<Stance>(Second, std::make_unique<BlockEverythingStance>())));

  ASSERT_FALSE(character_->ReceiveWeaponDamage(100, Weapon::Type::Scythe));
  ASSERT_EQ(character_->GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 1);

  ASSERT_FALSE(character_->ReceiveWeaponDamage(200, Weapon::Type::Staff));
  ASSERT_EQ(character_->GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 2);
}

TEST_F(CharacterTest, StartWithNoAdrenaline) {
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CharacterTest, OneHitBuildsOneStrikeOfAdrenalineOnAllSkills) {
  character_->WeaponAttack(*dummy_);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), Strike);
}

TEST_F(CharacterTest, GettingHitBuildsPercentageOfAdrenalineOnAllSkills) {
  character_->ReceiveWeaponDamage(character_->GetMaxHealth() / 2,
                                  Weapon::Type::Axe);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 2 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 2 * Strike);
}

TEST_F(CharacterTest, DeathTakesAllAdrenaline) {
  bonettis_defense_->AddAdrenaline(4 * Strike);
  barbarous_slice_->AddAdrenaline(2 * Strike);

  character_->ReceiveWeaponDamage(character_->GetMaxHealth(),
                                  Weapon::Type::Axe);
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CharacterTest, UsingSkillRemovesOneAdrenalineStrikeFromOthers) {
  bonettis_defense_->AddAdrenaline(10 * Strike);
  barbarous_slice_->AddAdrenaline(10 * Strike);

  World empty;

  bonettis_defense_->Activate(*character_, empty);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0 * Strike);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 9 * Strike);
}

TEST_F(CharacterTest, UsingSkillRemovesAllAdrenalingFromItself) {
  barbarous_slice_->AddAdrenaline(10 * Strike);

  World empty;

  barbarous_slice_->Activate(*character_, empty);

  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 0 * Strike);
}

TEST_F(CharacterTest, AddConditionAddsCondition) {
  character_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
}

TEST_F(CharacterTest, AddTwoConditionsAddsTwoCondition) {
  character_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
  character_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<DeepWound>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::DeepWound));
}

TEST_F(CharacterTest, SecondConditionOverridesIfLonger) {
  character_->AddCondition(
      Effect<Condition>(9 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
  auto bleeding_ = character_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_NE(bleeding_, nullptr);
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
}

TEST_F(CharacterTest, SecondConditionDoesNotOverrideIfShorter) {
  character_->AddCondition(
      Effect<Condition>(9 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
  auto bleeding_ = character_->AddCondition(
      Effect<Condition>(8 * Second, std::make_unique<Bleeding>()));
  ASSERT_EQ(bleeding_, nullptr);
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
  ASSERT_EQ(character_->ConditionsCount(), 1);
}

TEST_F(CharacterTest, ActivateSkillDoesDamage) {
  character_->target_ = dummy_;
  character_->UseSkill(pure_strike_, world());
  ASSERT_EQ(character_->GetActionType(), Action::Type::Busy);
  AwaitIdle(character_);
  ASSERT_NE(dummy_->GetLostHealth(), 0);
}

TEST_F(CharacterTest, StartWeaponAttackDoesDamage) {
  character_->target_ = dummy_;
  character_->StartWeaponAttack();
  ASSERT_EQ(character_->GetActionType(), Action::Type::Busy);
  AwaitIdle(character_);
  ASSERT_NE(dummy_->GetLostHealth(), 0);
}

TEST_F(CharacterTest, FleeingTakesYouHalfAggroCircleAway) {
  character_->FleeFrom(*dummy_);
  AwaitIdle(character_);
  ASSERT_FALSE(InRange(character_->GetPosition(), dummy_->GetPosition(),
                       EarshotRange / 2));
}

TEST_F(CharacterTest, ConditionsAreCountedCorrectly) {
  ASSERT_EQ(character_->ConditionsCount(), 0);
  character_->AddCondition(
      Effect<Condition>(1 * Millisecond, std::make_unique<Bleeding>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::Bleeding));
  ASSERT_EQ(character_->ConditionsCount(), 1);

  character_->AddCondition(
      Effect<Condition>(2 * Millisecond, std::make_unique<DeepWound>()));
  ASSERT_TRUE(character_->HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(character_->ConditionsCount(), 2);
  Tick();
  ASSERT_EQ(character_->ConditionsCount(), 1);
  Tick();
  ASSERT_EQ(character_->ConditionsCount(), 0);
}
