#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "character/stance.h"

void BonettisDefense::ActivationEnd(Creature& creature, World& world) {
  Time time = kDuration[creature.GetBuild().GetAttribute(kAttribute)] * Second;
  creature.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>()));

  Skill::ActivationEnd(creature, world);
};
