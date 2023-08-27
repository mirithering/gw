#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/profession.h"
#include "character/creature.h"
#include "character/skill.h"
#include "engine.h"
#include "skills/desperate_strike.h"
#include "skills/jaizenju_strike.h"
#include "skills/pure_strike.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

int kMaxTime = 60 * 1000;  // 60 Seconds game time.

int main() {
  Creature warrior(Profession::Warrior);
  warrior.SetAttribute(Attribute::Swordsmanship, 12);
  warrior.SetAttribute(Attribute::Strength, 12);
  warrior.SetSkill(0, std::make_unique<PureStrike>());
  warrior.SetSkill(1, std::make_unique<JaizenjuStrike>());

  Creature assassin(Profession::Assassin);
  assassin.SetAttribute(Attribute::DaggerMastery, 12);
  assassin.SetAttribute(Attribute::DeadlyArts, 12);
  assassin.SetSkill(0, std::make_unique<DesperateStrike>());

  warrior.GiveWeapon(std::make_unique<Sword>());
  assassin.GiveWeapon(std::make_unique<Dagger>());

  for (int ticks = 0; ticks < kMaxTime; ++ticks) {
    Tick(warrior, assassin, ticks);
    Tick(assassin, warrior, ticks);
  }
}