#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"

void BonettisDefense::ActivationEnd(Character& source) {
  int time = kDuration[source.GetAttribute(kAttribute)] * 1000;
  source.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>(source)));

  Skill::ActivationEnd(source);
};

int BonettisDefenseStance::BlockChance(Weapon::Type type) {
  if (IsMeele(type) || IsProjectile(type)) {
    return 75;
  }
  return 0;
}

void BonettisDefenseStance::AttackBlocked(Weapon::Type type) {
  if (IsMeele(type)) {
    character_.AddEnergy(5);
  }
}