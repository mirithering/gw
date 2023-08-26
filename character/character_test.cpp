#include "character.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/profession.h"
#include "skills/bonettis_defense.h"
#include "weapon/sword.h"

TEST(CharacterTest, GetAttributeZeroIfUnset) {
  Character character(Profession::Warrior);
  EXPECT_EQ(character.GetAttribute(Attribute::AxeMastery), 0);
}

TEST(CharacterTest, WarriorRecharges2EnergyEvery3000Ticks) {
  Character character(Profession::Warrior);
  character.UseEnergy(5);
  int energy = character.energy();
  for (int i = 0; i < 3000; ++i) Tick();
  EXPECT_EQ(character.energy(), energy + 2);
}

TEST(CharacterTest, SetAttributeSetsAttribute) {
  Character character(Profession::Warrior);
  character.SetAttribute(Attribute::Strength, 12);
  EXPECT_EQ(character.GetAttribute(Attribute::Strength), 12);
}

TEST(CharacterTest, StartWithNoAdrenaline) {
  Character character(Profession::Warrior);
  auto skill_ptr = std::make_unique<BonettisDefense>();
  auto skill = skill_ptr.get();
  character.SetSkill(0, std::move(skill_ptr));
  EXPECT_EQ(skill->GetAdrenaline(), 0);
}

TEST(CharacterTest, StanceBlocksAttacks) {
  class BlockEverythingStance : public Stance {
   public:
    BlockEverythingStance() {}
    int BlockChance(Weapon::Type) override { return 100; }
    void AttackBlocked(Weapon::Type) override { ++attack_blocked_called_; }
    int attack_blocked_called_ = 0;
  };

  Character character(Profession::Warrior);
  BlockEverythingStance* stance =
      static_cast<BlockEverythingStance*>(character.SetStance(
          Effect<Stance>(10, std::make_unique<BlockEverythingStance>())));

  EXPECT_FALSE(character.ReceiveWeaponDamage(100, Weapon::Type::Scythe));
  EXPECT_EQ(character.GetLostHealth(), 0);
  EXPECT_EQ(stance->attack_blocked_called_, 1);

  EXPECT_FALSE(character.ReceiveWeaponDamage(200, Weapon::Type::Staff));
  EXPECT_EQ(character.GetLostHealth(), 0);
  EXPECT_EQ(stance->attack_blocked_called_, 2);
}

class AdrenalineCharacterTest : public ::testing::Test {
  void SetUp() override {
    auto skill_ptr1 = std::make_unique<BonettisDefense>();
    adrenaline_skill1 = skill_ptr1.get();
    character.SetSkill(0, std::move(skill_ptr1));

    auto skill_ptr2 = std::make_unique<BonettisDefense>();
    adrenaline_skill2 = skill_ptr2.get();
    character.SetSkill(1, std::move(skill_ptr2));

    character.GiveWeapon(std::make_unique<Sword>());
  }

 protected:
  Character character{Profession::Warrior};
  Skill* adrenaline_skill1;
  Skill* adrenaline_skill2;
  Character dummy{Profession::Warrior};
};

TEST_F(AdrenalineCharacterTest, OneHitBuildsOneStrikeOfAdrenalineOnAllSkills) {
  character.WeaponAttack(dummy);

  EXPECT_EQ(adrenaline_skill1->GetAdrenaline(), 25);
  EXPECT_EQ(adrenaline_skill2->GetAdrenaline(), 25);
}

TEST_F(AdrenalineCharacterTest,
       GettingHitBuildsPercentageOfAdrenalineOnAllSkills) {
  character.ReceiveWeaponDamage(character.GetMaxHealth() / 2,
                                Weapon::Type::Axe);

  EXPECT_EQ(adrenaline_skill1->GetAdrenaline(), 50);
  EXPECT_EQ(adrenaline_skill2->GetAdrenaline(), 50);
}

TEST_F(AdrenalineCharacterTest, DeathTakesAllAdrenaline) {
  adrenaline_skill1->AddAdrenaline(100);
  adrenaline_skill2->AddAdrenaline(50);

  character.ReceiveWeaponDamage(character.GetMaxHealth(), Weapon::Type::Axe);
  EXPECT_EQ(adrenaline_skill1->GetAdrenaline(), 0);
  EXPECT_EQ(adrenaline_skill2->GetAdrenaline(), 0);
}

TEST_F(AdrenalineCharacterTest,
       RemoveOneAdrenalineStrikeRemovesAdrenalineStrike) {
  adrenaline_skill1->AddAdrenaline(1000);
  adrenaline_skill2->AddAdrenaline(1000);

  adrenaline_skill1->Activate(character, dummy);

  EXPECT_EQ(adrenaline_skill2->GetAdrenaline(), 1000 - 25);
}