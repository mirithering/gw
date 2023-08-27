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
  int kTime = 37907;

  Creature attacker = ConstructCreature(
      Profession::Warrior, Sword(),
      {{Attribute::Swordsmanship, 12}, {Attribute::Strength, 12}}, PureStrike(),
      JaizenjuStrike(), BarbarousSlice(), Gash());
  attacker.name_ = "Attacker";

  Creature defender =
      ConstructCreature(Profession::Warrior, Sword(),
                        {{Attribute::Tactics, 12}}, BonettisDefense());
  defender.GetBuild().SetArmor(std::make_unique<Armor>(0));
  defender.name_ = "Defender";

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(defender.GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    ASSERT_NE(attacker.GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    Tick();
    Tick(attacker, defender, ticks);
    Tick(defender, attacker, ticks);
  }
  ASSERT_EQ(defender.GetAction().GetType(), Action::Type::Dead);
}
