#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "character/character.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/staff.h"
#include "weapon/sword.h"

Character* AddWarriorTo(std::vector<std::unique_ptr<Character>>& group) {
  group.push_back(std::make_unique<Character>(
      ConstructBuild(Profession::Warrior, std::make_unique<Sword>())));
  group.back()->name_ = "Warrior";
  return group.back().get();
}

Character* AddRangerTo(std::vector<std::unique_ptr<Character>>& group) {
  group.push_back(std::make_unique<Character>(
      ConstructBuild(Profession::Ranger, std::make_unique<Flatbow>())));
  group.back()->name_ = "Ranger";
  return group.back().get();
}

Character* AddAssassinTo(std::vector<std::unique_ptr<Character>>& group) {
  group.push_back(std::make_unique<Character>(
      ConstructBuild(Profession::Assassin, std::make_unique<Dagger>())));
  group.back()->name_ = "Assassin";
  return group.back().get();
}

Character* AddMesmerTo(std::vector<std::unique_ptr<Character>>& group) {
  group.push_back(std::make_unique<Character>(ConstructBuild(
      Profession::Mesmer,
      std::make_unique<Staff>(DamageType::Chaos, Attribute::IllusionMagic))));
  group.back()->name_ = "Mesmer";
  return group.back().get();
}

void AwaitIdle(Character* character) {
  while (character->GetActionType() != Action::Type::Idle) {
    Tick();
  }
}