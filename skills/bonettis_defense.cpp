#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "character/stance.h"

void BonettisDefense::ActivationEnd(Creature& creature,
                                    std::vector<Creature>& my_team,
                                    std::vector<Creature>& enemy_team) {
  int time = kDuration[creature.GetBuild().GetAttribute(kAttribute)] * 1000;
  creature.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>(creature)));

  Skill::ActivationEnd(creature, my_team, enemy_team);
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