#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "entities/creature.h"
#include "entities/skill.h"
#include "entities/stance.h"
#include "entities/world.h"

void BonettisDefense::ActivationEnd(Creature& creature, World& world) {
  Time time = kDuration[creature.GetBuild().GetAttribute(kAttribute)] * Second;
  creature.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>()));

  Skill::ActivationEnd(creature, world);
};
