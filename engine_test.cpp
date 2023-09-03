#include "engine.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/attribute.h"
#include "base/logging.h"
#include "base/profession.h"
#include "character/creature.h"
#include "character/skill.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/attack/sword/gash.h"
#include "skills/attack/sword/jaizenju_strike.h"
#include "skills/attack/sword/pure_strike.h"
#include "skills/bonettis_defense.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

// TODO this test needs to change when something about random changes, but it
// guarantees I am not changing anything else.
TEST(EngineTest, WarriorWithSkillsKillsWarriorInExactlyXTurns) {
  int kTime = 33916;  // TODO I think there are some mistakes in my code...

  std::vector<Creature> team;
  std::vector<Creature> enemies;

  team.push_back(ConstructCreature(
      Profession::Warrior, Sword(),
      {{Attribute::Swordsmanship, 12}, {Attribute::Strength, 12}}, PureStrike(),
      JaizenjuStrike(), BarbarousSlice(), Gash()));
  team.back().name_ = "Attacker";

  enemies.push_back(ConstructCreature(Profession::Warrior, Sword(),
                                      {{Attribute::Tactics, 12}},
                                      BonettisDefense()));
  // enemies.back().GetBuild().SetArmor(std::make_unique<Armor>(0));
  enemies.back().name_ = "Defender";

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(enemies.back().GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    ASSERT_NE(team.back().GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    Tick();
    NextActions(team, enemies);
  }
  ASSERT_EQ(enemies.back().GetAction().GetType(), Action::Type::Dead);
}

TEST(EngineTest, RangerAgainstWarriorAttackTiming) {
  std::vector<Creature> team;
  std::vector<Creature> enemies;

  team.push_back(ConstructCreature(
      Profession::Warrior, Sword(),
      {{Attribute::Swordsmanship, 12}, {Attribute::Strength, 12}}));
  team.back().name_ = "Warrior";

  enemies.push_back(ConstructCreature(Profession::Ranger, Flatbow(),
                                      {{Attribute::Marksmanship, 12}}));
  enemies.back().name_ = "Ranger";

  Creature& warrior = team.back();
  Creature& ranger = enemies.back();

  int expected_ticks_to_warrior_attack = 1330 / 2;
  int expected_ticks_to_warrior_idle = 1330;
  int expected_ticks_to_ranger_idle = 2025;
  int expected_ticks_to_ranger_attack = 2025 + 880;
  NextActions(team, enemies);

  int ticks = 0;
  for (; ticks < expected_ticks_to_warrior_attack; ++ticks) {
    ASSERT_EQ(warrior.GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger.GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger.GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior.GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_warrior_idle; ++ticks) {
    ASSERT_EQ(warrior.GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger.GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_NE(ranger.GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior.GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_ranger_idle; ++ticks) {
    ASSERT_EQ(warrior.GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_EQ(ranger.GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_NE(ranger.GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior.GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_ranger_attack; ++ticks) {
    ASSERT_EQ(warrior.GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_EQ(ranger.GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_NE(ranger.GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior.GetLostHealth(), 0) << ticks;
    Tick();
  }
  ASSERT_NE(warrior.GetLostHealth(), 0) << ticks;
}
