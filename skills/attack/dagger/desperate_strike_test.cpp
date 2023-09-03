#include "desperate_strike.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

TEST(DesparateStrikeTest, AdditionalDamageIfBelowHealth) {
  std::vector<Creature> team, enemies;
  team.push_back(ConstructCreature(Profession::Assassin, Dagger(),
                                   {{Attribute::DaggerMastery, 12}},
                                   DesperateStrike()));
  Creature& assassin = team[0];
  DesperateStrike* desperate_strike =
      assassin.GetBuild().GetSkill<DesperateStrike>(0);

  enemies.push_back(ConstructCreature(Profession::Warrior, Sword()));
  Creature& dummy = enemies[0];
  // 12 * 5 = 60
  dummy.GetBuild().SetArmor(std::make_unique<Armor>(60));

  OverrideRandomValueForTesting(12);
  int weapon_damage = 12;

  assassin.LoseHealth(of(assassin.GetMaxHealth(), Percent(27)));
  int skill_damage = 51;

  assassin.target_ = &dummy;
  assassin.GetAction() = desperate_strike->Activate(assassin, team, enemies);
  while (assassin.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_EQ(dummy.GetLostHealth(), weapon_damage + skill_damage);
}

TEST(DesparateStrikeTest, NoAdditionalDamageIfNotBelowHealth) {
  std::vector<Creature> team, enemies;
  team.push_back(ConstructCreature(Profession::Assassin, Dagger(),
                                   {{Attribute::DaggerMastery, 9}},
                                   DesperateStrike()));
  Creature& assassin = team[0];
  DesperateStrike* desperate_strike =
      assassin.GetBuild().GetSkill<DesperateStrike>(0);

  enemies.push_back(ConstructCreature(Profession::Warrior, Sword()));
  Creature& dummy = enemies[0];
  // 9 * 5 = 45
  dummy.GetBuild().SetArmor(std::make_unique<Armor>(45));

  OverrideRandomValueForTesting(12);
  int weapon_damage = 12;

  assassin.LoseHealth(of(assassin.GetMaxHealth(), Percent(25)));
  int skill_damage = 0;

  assassin.target_ = &dummy;
  assassin.GetAction() = desperate_strike->Activate(assassin, team, enemies);
  while (assassin.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  EXPECT_EQ(dummy.GetLostHealth(), weapon_damage + skill_damage);
}