#include "soothing_images.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
#include "base/units.h"
#include "entities/creature.h"
#include "skills/attack/sword/gash.h"
#include "test/test.h"
#include "weapon/sword.h"

class SoothingImagesTest : public GwTest {
 public:
  void SetUp() override {
    mesmer_ = AddMesmerTo(team());
    soothing_images_ =
        mesmer_->GetBuild().AddSkill(std::make_unique<SoothingImages>());

    enemy_ = AddWarriorTo(enemies());
    mesmer_->target_ = enemy_;
  }

 protected:
  Creature* mesmer_;
  SoothingImages* soothing_images_;

  Creature* enemy_;
};

TEST_F(SoothingImagesTest, TargetMovesCannotGainAdrenaline) {
  auto gash = enemy_->GetBuild().AddSkill(std::make_unique<Gash>());
  EXPECT_EQ(gash->GetAdrenaline(), Adrenaline(0));

  enemy_->ReceiveWeaponDamage(enemy_->GetMaxHealth() / 4, Weapon::Type::Sword);
  EXPECT_NE(gash->GetAdrenaline(), Adrenaline(0));
  Adrenaline previous_adrenaline = gash->GetAdrenaline();

  mesmer_->UseSkill(soothing_images_, world());
  AwaitIdle(mesmer_);

  enemy_->ReceiveWeaponDamage(enemy_->GetMaxHealth() / 4, Weapon::Type::Sword);
  EXPECT_EQ(gash->GetAdrenaline(), previous_adrenaline);
}

TEST_F(SoothingImagesTest, AffectsAdjacentEnemies) {
  Creature* adjacent_enemy_ = AddWarriorTo(enemies());
  adjacent_enemy_->SetPosition(Position({Inches(100), Inches(100)}));

  Creature* away_enemy_ = AddWarriorTo(enemies());
  away_enemy_->SetPosition(Position({Inches(140), Inches(140)}));

  mesmer_->UseSkill(soothing_images_, world());
  AwaitIdle(mesmer_);

  ASSERT_TRUE(enemy_->IsHexed());
  ASSERT_TRUE(adjacent_enemy_->IsHexed());
  ASSERT_FALSE(away_enemy_->IsHexed());
}

TEST_F(SoothingImagesTest, TargetWarriorIfCurrentTargetIsNoWarrior) {
  Creature* assassin = AddAssassinTo(enemies());
  assassin->SetPosition(Position({Inches(140), Inches(140)}));

  mesmer_->target_ = assassin;

  mesmer_->UseSkill(soothing_images_, world());
  AwaitIdle(mesmer_);

  EXPECT_TRUE(enemy_->IsHexed());
  EXPECT_FALSE(assassin->IsHexed());
}
