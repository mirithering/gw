#include "desperate_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "test/test.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

class DesperateStrikeTest : public GwTest {
 public:
  void SetUp() override {
    assassin = AddAssassinTo(team());
    desperate_strike =
        assassin->GetBuild().AddSkill(std::make_unique<DesperateStrike>());

    enemy = AddWarriorTo(enemies());
  }

 protected:
  Creature* assassin;
  Creature* enemy;
  DesperateStrike* desperate_strike;
};

TEST_F(DesperateStrikeTest, AdditionalDamageIfBelowHealth) {
  assassin->GetBuild().SetAttribute(Attribute::DaggerMastery, 12);
  // 12 * 5 = 60
  enemy->GetBuild().SetArmor(std::make_unique<Armor>(60));

  OverrideRandomValueForTesting(12);
  int weapon_damage = 12;

  assassin->LoseHealth(of(assassin->GetMaxHealth(), Percent(27)));
  int skill_damage = 51;

  assassin->target_ = enemy;
  assassin->UseSkill(desperate_strike, world());
  AwaitIdle(assassin);
  EXPECT_EQ(enemy->GetLostHealth(), weapon_damage + skill_damage);
}

TEST_F(DesperateStrikeTest, NoAdditionalDamageIfNotBelowHealth) {
  assassin->GetBuild().SetAttribute(Attribute::DaggerMastery, 9);
  // 9 * 5 = 45 -> damage comes through equally.
  enemy->GetBuild().SetArmor(std::make_unique<Armor>(45));

  OverrideRandomValueForTesting(12);
  int weapon_damage = 12;

  assassin->LoseHealth(of(assassin->GetMaxHealth(), Percent(25)));
  int skill_damage = 0;

  assassin->target_ = enemy;
  assassin->UseSkill(desperate_strike, world());
  AwaitIdle(assassin);
  EXPECT_EQ(enemy->GetLostHealth(), weapon_damage + skill_damage);
}