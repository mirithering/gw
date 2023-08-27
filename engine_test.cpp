#include "engine.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/attribute.h"
#include "base/profession.h"
#include "character/creature.h"
#include "character/skill.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/attack/sword/gash.h"
#include "skills/attack/sword/jaizenju_strike.h"
#include "skills/attack/sword/pure_strike.h"
#include "skills/bonettis_defense.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

// TODO this test needs to change when something about random changes, but it
// guarantees I am not changing anything else.
TEST(EngineTest, WarriorWithSkillsKillsWarriorInExactlyXTurns) {
  int kTime = 33917;  // TODO I think there are some mistakes in my code...

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
