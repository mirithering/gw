#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "character/creature.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

Creature* AddWarriorTo(std::vector<std::unique_ptr<Creature>>& group) {
  group.push_back(std::make_unique<Creature>(
      ConstructBuild(Profession::Warrior, std::make_unique<Sword>())));
  group.back()->name_ = "Warrior";
  return group.back().get();
}

Creature* AddRangerTo(std::vector<std::unique_ptr<Creature>>& group) {
  group.push_back(std::make_unique<Creature>(
      ConstructBuild(Profession::Ranger, std::make_unique<Flatbow>())));
  group.back()->name_ = "Ranger";
  return group.back().get();
}

Creature* AddAssassinTo(std::vector<std::unique_ptr<Creature>>& group) {
  group.push_back(std::make_unique<Creature>(
      ConstructBuild(Profession::Assassin, std::make_unique<Dagger>())));
  group.back()->name_ = "Warrior";
  return group.back().get();
}

void TickUntilIdle(Creature* creature) {
  while (creature->GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
}