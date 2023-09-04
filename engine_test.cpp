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
#include "test/test.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

class EngineTest : public GwTest {};

// TODO this test needs to change when something about random changes, but it
// guarantees I am not changing anything else.
TEST_F(EngineTest, WarriorWithSkillsKillsWarriorInExactlyXTurns) {
  int kTime = 33916;

  auto attacker = AddWarriorTo(team());
  attacker->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
  attacker->GetBuild().SetAttribute(Attribute::Strength, 12);
  attacker->GetBuild().SetSkills(
      std::make_unique<PureStrike>(), std::make_unique<JaizenjuStrike>(),
      std::make_unique<BarbarousSlice>(), std::make_unique<Gash>());
  attacker->name_ = "Attacker";

  auto defender = AddWarriorTo(enemies());
  defender->GetBuild().SetAttribute(Attribute::Tactics, 12);
  defender->GetBuild().AddSkill(std::make_unique<BonettisDefense>());
  defender->name_ = "Defender";

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(attacker->GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    ASSERT_NE(defender->GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    Tick();
    NextActions(world());
  }
  ASSERT_EQ(defender->GetAction().GetType(), Action::Type::Dead);
}

TEST_F(EngineTest, RangerAgainstWarriorAttackTiming) {
  auto warrior = AddWarriorTo(team());
  warrior->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
  warrior->GetBuild().SetAttribute(Attribute::Strength, 12);

  auto ranger = AddRangerTo(enemies());
  ranger->GetBuild().SetAttribute(Attribute::Marksmanship, 12);

  int expected_ticks_to_warrior_attack = 1330 / 2;
  int expected_ticks_to_warrior_idle = 1330;
  int expected_ticks_to_ranger_idle = 2025;
  int expected_ticks_to_ranger_attack = 2025 + 880;
  NextActions(world());

  int ticks = 0;
  for (; ticks < expected_ticks_to_warrior_attack; ++ticks) {
    ASSERT_EQ(warrior->GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger->GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger->GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior->GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_warrior_idle; ++ticks) {
    ASSERT_EQ(warrior->GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_EQ(ranger->GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_NE(ranger->GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior->GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_ranger_idle; ++ticks) {
    ASSERT_EQ(warrior->GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_EQ(ranger->GetAction().GetType(), Action::Type::Busy) << ticks;
    ASSERT_NE(ranger->GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior->GetLostHealth(), 0) << ticks;
    Tick();
  }
  for (; ticks < expected_ticks_to_ranger_attack; ++ticks) {
    ASSERT_EQ(warrior->GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_EQ(ranger->GetAction().GetType(), Action::Type::Idle) << ticks;
    ASSERT_NE(ranger->GetLostHealth(), 0) << ticks;
    ASSERT_EQ(warrior->GetLostHealth(), 0) << ticks;
    Tick();
  }
  ASSERT_NE(warrior->GetLostHealth(), 0) << ticks;
}
