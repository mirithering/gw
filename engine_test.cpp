#include "engine.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/attribute.h"
#include "base/profession.h"
#include "character/character.h"
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
  int kTime = 32587;

  Character attacker(Profession::Warrior);
  attacker.name_ = "Attacker";
  attacker.SetAttribute(Attribute::Swordsmanship, 12);
  attacker.SetAttribute(Attribute::Strength, 12);
  attacker.SetSkill(0, std::make_unique<PureStrike>());
  attacker.SetSkill(1, std::make_unique<JaizenjuStrike>());
  attacker.SetSkill(2, std::make_unique<BarbarousSlice>());
  attacker.SetSkill(3, std::make_unique<Gash>());

  Character defender(Profession::Warrior);
  defender.name_ = "Defender";
  defender.SetAttribute(Attribute::Tactics, 12);
  defender.SetSkill(0, std::make_unique<BonettisDefense>());

  attacker.GiveWeapon(std::make_unique<Sword>());
  defender.GiveWeapon(std::make_unique<Sword>());

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(defender.GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    ASSERT_NE(attacker.GetAction().GetType(), Action::Type::Dead)
        << " " << ticks;
    Tick();
    Tick(attacker, defender, ticks);
    Tick(defender, attacker, ticks);
  }
  EXPECT_EQ(defender.GetAction().GetType(), Action::Type::Dead);
}
