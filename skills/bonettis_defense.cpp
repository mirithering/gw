#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "character/stance.h"

void BonettisDefense::ActivationEnd(
    Creature& creature, std::vector<std::unique_ptr<Creature>>& my_team,
    std::vector<std::unique_ptr<Creature>>& enemy_team) {
  Time time = kDuration[creature.GetBuild().GetAttribute(kAttribute)] * Second;
  creature.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>()));

  Skill::ActivationEnd(creature, my_team, enemy_team);
};
