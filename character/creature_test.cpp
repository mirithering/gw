#include "creature.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/profession.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/bonettis_defense.h"
#include "weapon/sword.h"

class CreatureTest : public ::testing::Test {
 public:
  void SetUp() override {
    bonettis_defense_ = creature_.GetBuild().GetSkill<BonettisDefense>(0);
    ;
    barbarous_slice_ = creature_.GetBuild().GetSkill<BarbarousSlice>(1);
  }

 protected:
  BonettisDefense* bonettis_defense_;
  BarbarousSlice* barbarous_slice_;
  Creature creature_ = ConstructCreature(Profession::Warrior, Sword(), {},
                                         BonettisDefense(), BarbarousSlice());
  Creature dummy_ = ConstructCreature(Profession::Warrior, Sword(), {});
  ;
};

TEST_F(CreatureTest, WarriorRecharges2EnergyEvery3000Ticks) {
  creature_.UseEnergy(5);
  int energy = creature_.energy();
  for (int i = 0; i < 3000; ++i) Tick();
  ASSERT_EQ(creature_.energy(), energy + 2);
}

TEST_F(CreatureTest, StanceBlocksAttacks) {
  class BlockEverythingStance : public Stance {
   public:
    BlockEverythingStance() {}
    int BlockChance(Weapon::Type) override { return 100; }
    void AttackBlocked(Weapon::Type) override { ++attack_blocked_called_; }
    int attack_blocked_called_ = 0;
  };

  BlockEverythingStance* stance =
      static_cast<BlockEverythingStance*>(creature_.SetStance(
          Effect<Stance>(10, std::make_unique<BlockEverythingStance>())));

  ASSERT_FALSE(creature_.ReceiveWeaponDamage(100, Weapon::Type::Scythe));
  ASSERT_EQ(creature_.GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 1);

  ASSERT_FALSE(creature_.ReceiveWeaponDamage(200, Weapon::Type::Staff));
  ASSERT_EQ(creature_.GetLostHealth(), 0);
  ASSERT_EQ(stance->attack_blocked_called_, 2);
}

TEST_F(CreatureTest, StartWithNoAdrenaline) {
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0);
}

TEST_F(CreatureTest, OneHitBuildsOneStrikeOfAdrenalineOnAllSkills) {
  creature_.WeaponAttack(dummy_);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 25);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 25);
}

TEST_F(CreatureTest, GettingHitBuildsPercentageOfAdrenalineOnAllSkills) {
  creature_.ReceiveWeaponDamage(creature_.GetMaxHealth() / 2,
                                Weapon::Type::Axe);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 50);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 50);
}

TEST_F(CreatureTest, DeathTakesAllAdrenaline) {
  bonettis_defense_->AddAdrenaline(100);
  barbarous_slice_->AddAdrenaline(50);

  creature_.ReceiveWeaponDamage(creature_.GetMaxHealth(), Weapon::Type::Axe);
  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 0);
}

TEST_F(CreatureTest, UsingSkillRemovesOneAdrenalineStrike) {
  bonettis_defense_->AddAdrenaline(1000);
  barbarous_slice_->AddAdrenaline(1000);

  std::vector<Creature> empty;

  bonettis_defense_->Activate(creature_, empty, empty);

  ASSERT_EQ(bonettis_defense_->GetAdrenaline(), 0);
  ASSERT_EQ(barbarous_slice_->GetAdrenaline(), 1000 - 25);
}